/*! \file accelerometer.c
*
*  \brief contains the implementation of the LIS302DL 3-Axis MEMS accelerometer driver.
*
*
*  \author Graham Riches
*/


/********************************** Includes *******************************************/
#include "accelerometer.h"
#include "stm32f4xx_hal.h"
#include "spi.h"
#include "debug.h"
#include "board.h"
#include "event.h"
#include "gpio.h"
#include "arm_math.h"
#include <string.h>

/*********************************** Consts ********************************************/
/* LIS302DL API definitions */
#define ACCEL_WRITE                 0x00U //!< write bit
#define ACCEL_READ                  0x80U //!< read bit
#define ACCEL_WAKE_UP_100HZ         0x57U //!< wake-up command at 100 Hz
#define ACCEL_WAKE_UP_200HZ         0x67U //!< wake-up command at 200 Hz
#define ACCEL_WAKE_UP_400HZ         0x77U //!< wake-up command at 400 Hz
#define ACCEL_ENABLE_DR_INT         0xC8U //!< enable data ready interrupt
#define ACCEL_ENABLE_MULTIBYTE_READ 0x10U //!< enable multi-byte read chaining
#define ACCEL_WHO_AM_I_EXP          0x3FU //!< expected value of the WHO_AM_I register

/* Data read/accessing */
#define ACCEL_READ_DATA_CMD_SIZE    7 //!< need 7 bytes to read all the data registers -> CMD XL XH YL YH ZL ZH
#define ACCEL_WRITE_PAYLOAD_SIZE    2 //!< 2 bytes for a write command
#define ACCEL_ADDRESS_INDEX         0 //!< address is the first byte
#define ACCEL_CMD_INDEX             1 //!< command is the second byte
#define ACCEL_X_DATA_INDEX          1 //!< x data index
#define ACCEL_Y_DATA_INDEX          3 //!< y data index
#define ACCEL_Z_DATA_INDEX          5 //!< z data index

/* Register Addresses */
#define ACCEL_WHO_AM_I  0x0FU //!< device ID register
#define ACCEL_CTRL_REG1 0x21U //!< control register 1
#define ACCEL_CTRL_REG2 0x22U //!< control register 2
#define ACCEL_CTRL_REG3 0x23U //!< control register 3
#define ACCEL_CTRL_REG4 0x20U //!< control register 4
#define ACCEL_CTRL_REG5 0x24U //!< control register 5
#define ACCEL_CTRL_REG6 0x25U //!< control register 6
#define ACCEL_SR_REG    0x27U //!< device status register
#define ACCEL_OUT_X_L   0x28U //!< acceleration output low-byte in X-axis
#define ACCEL_OUT_X_H   0x29U //!< acceleration output low-byte in X-axis
#define ACCEL_OUT_Y_L   0x2AU //!< acceleration output low-byte in Y-axis
#define ACCEL_OUT_Y_H   0x2BU //!< acceleration output low-byte in Y-axis
#define ACCEL_OUT_Z_L   0x2CU //!< acceleration output low-byte in Z-axis
#define ACCEL_OUT_Z_H   0x2DU //!< acceleration output low-byte in Z-axis

/* Constants and conversions */
#define ACCEL_2G_SCALE_FACTOR    0x4000  //!< convert from +/- 2g int data to floating point
#define G_TO_METER_PER_S2        9.81    //!< gravitational conversion from [g] to [m/s2]
#define ACCEL_LED_THRESHOLD      0.1     //!< trigger level to light the LEDs

/* Filter and Buffer Setup */
#define ACCEL_BUFFER_SIZE        64     //!< number of samples per data buffer
#define ACCEL_BUFF_BANK_0        0       //!< first data buffer
#define ACCEL_BUFF_BANK_1        1       //!< second data buffer
#define ACCEL_DSP_FILTER_TAPS    20      //!< arm DSP FIR taps


/************************************ Types ********************************************/
/**
 * \brief handler structure for processing accelerometer data in banks
 */
typedef struct
{
    float rawBank_0[ACCEL_BUFFER_SIZE]; //!< first raw data bank
    float rawBank_1[ACCEL_BUFFER_SIZE]; //!< second raw data bank
    float bank_0[ACCEL_BUFFER_SIZE];    //!< first processed data buffer bank
    float bank_1[ACCEL_BUFFER_SIZE];    //!< second processed buffer bank
    float *writePtr;                    //!< current raw data write pointer
    float *readPtr;                     //!< read pointer
} ACCEL_dualBank_t;

/**
 * \brief structure to manage X,Y,Z data buffering and processing
 */
typedef struct
{
    ACCEL_dualBank_t x; //!< x data buffer
    ACCEL_dualBank_t y; //!< y data buffer
    ACCEL_dualBank_t z; //!< z data buffer
    uint16_t samples;   //!< samples read
    uint8_t activeBank; //!< active bank index
} ACCEL_dataHandler_t;


/**
 * \brief structure to control different accelerometer feature flags
 */
typedef struct
{
    bool print;      //!< enable/disable uart output
    bool ledControl; //!< enable/disable LED control mode
} ACCEL_control_t;

/*********************************** Macros ********************************************/
#define REG_TO_RAWDATA(x, y) ((float)((int16_t)((uint16_t)x  | ((uint16_t)y << 8)))/ACCEL_2G_SCALE_FACTOR)

/******************************* Global Variables **************************************/

/******************************** DSP Filter Parameters **************************************/
/* NOTE: all filters are currently using the the same parameters, however each signal needs its own filter
 *       object and state array.
 */
float filtCoeffs[ACCEL_DSP_FILTER_TAPS] = { 0.000511, 0.002174, 0.006474, 0.015337, 0.029753, 0.049215,
                                            0.071579, 0.093438, 0.110905, 0.120614, 0.120614, 0.110905,
                                            0.093438, 0.071579, 0.049215, 0.029753, 0.015337, 0.006474,
                                            0.002174, 0.000511 };

float xFiltStateArr[ACCEL_DSP_FILTER_TAPS + ACCEL_BUFFER_SIZE - 1] = {0};
float yFiltStateArr[ACCEL_DSP_FILTER_TAPS + ACCEL_BUFFER_SIZE - 1] = {0};
float zFiltStateArr[ACCEL_DSP_FILTER_TAPS + ACCEL_BUFFER_SIZE - 1] = {0};

arm_fir_instance_f32 xFilter;
arm_fir_instance_f32 yFilter;
arm_fir_instance_f32 zFilter;


/******************************** Local Variables **************************************/
static ACCEL_dataHandler_t buffer = {0};
static ACCEL_control_t controller = {0};

/****************************** Functions Prototype ************************************/
void ACCEL_write( uint8_t addr, uint8_t command );
void ACCEL_onDataUpdate( void );

/****************************** Functions Definition ***********************************/
/**
* \name     ACCELEROMETER_init
* \brief    Initialize the module variables and resources.
*
* \param    None
* \retval   None
*/
void ACCEL_init( void )
{
    /* configure the external data ready interrupt */
    HAL_NVIC_SetPriority( USART3_IRQn, 8, 8 );
    HAL_NVIC_EnableIRQ( EXTI0_IRQn );

    /* write to control register 4 to wake up the device */
    ACCEL_write( ACCEL_CTRL_REG4, ACCEL_WAKE_UP_400HZ );

    /* enable the data ready interrupt */
    ACCEL_write( ACCEL_CTRL_REG3, ACCEL_ENABLE_DR_INT );

    /* enable multi-byte reading */
    ACCEL_write( ACCEL_CTRL_REG6, ACCEL_ENABLE_MULTIBYTE_READ );

    /* set the data processing buffer pointers to the first bank */
    buffer.x.writePtr = buffer.x.rawBank_0;
    buffer.y.writePtr = buffer.y.rawBank_0;
    buffer.z.writePtr = buffer.z.rawBank_0;

    /* set the read pointers the first instance of processed data */
    buffer.x.readPtr = buffer.x.bank_0;
    buffer.y.readPtr = buffer.y.bank_0;
    buffer.z.readPtr = buffer.z.bank_0;

    /* initialize the filter objects */
    arm_fir_init_f32( &xFilter, ACCEL_DSP_FILTER_TAPS, filtCoeffs, xFiltStateArr, ACCEL_BUFFER_SIZE);
    arm_fir_init_f32( &yFilter, ACCEL_DSP_FILTER_TAPS, filtCoeffs, yFiltStateArr, ACCEL_BUFFER_SIZE);
    arm_fir_init_f32( &zFilter, ACCEL_DSP_FILTER_TAPS, filtCoeffs, zFiltStateArr, ACCEL_BUFFER_SIZE);

    /* enable LED control mode */
    controller.ledControl = true;

    return;
}


/**
 * \brief set the accelerometer control mode
 * \retval None
 */
void ACCEL_setControlMode( ACCEL_controlMode_t mode )
{
    if ( mode < ACCEL_CONTROL_TOTAL_MODES )
    {
        switch ( mode )
        {
        case ACCEL_CONTROL_MODE_LED:
            controller.ledControl = true;
            break;

        case ACCEL_CONTROL_MODE_NONE:
            controller.ledControl = false;
            break;

        default:
            break;
        }
    }
}


/**
 * \brief read the devices WHO_AM_I register as a test and validate that it equals the expected
 *        value
 */
void ACCEL_test( void )
{
    uint16_t command = (ACCEL_WHO_AM_I | ACCEL_READ);
    uint16_t data = 0;
    SPI_readWrite( SPI_ACCELEROMETER, (uint8_t *)&command, (uint8_t *)&data, sizeof(command) );
    /* NOTE: the data we are interested in is in the second received byte */
    DEBUG_print("ACCELEROMETER TEST: Expected: %d Received: %d\n", ACCEL_WHO_AM_I_EXP, (uint8_t)(data >> 8));
    return;
}


/**
 * \brief write to a data register on the device
 * \param addr the register to write to
 * \command the data to write to the register
 */
void ACCEL_write( uint8_t addr, uint8_t command )
{
    uint8_t payload[ACCEL_WRITE_PAYLOAD_SIZE] = {0};
    payload[ACCEL_ADDRESS_INDEX] = (addr | ACCEL_WRITE);
    payload[ACCEL_CMD_INDEX] = command;
    SPI_write( SPI_ACCELEROMETER, payload, sizeof(payload) );
}


/**
 * \brief read the raw accelerometer data back from the device into the storage buffers
 * \NOTE the device registers are all sequential so it is faster
 *       to read them all in a chain.
 */
void ACCEL_readData( void )
{
    /* read the device data registers */
    uint8_t readCmd[ACCEL_READ_DATA_CMD_SIZE] = {0};
    uint8_t data[ACCEL_READ_DATA_CMD_SIZE] = {0};

    readCmd[0] = (ACCEL_READ | ACCEL_OUT_X_L );
    SPI_readWrite( SPI_ACCELEROMETER, readCmd, data, sizeof(readCmd) );
    *buffer.x.writePtr++ = REG_TO_RAWDATA(data[ACCEL_X_DATA_INDEX], data[ACCEL_X_DATA_INDEX + 1]);
    *buffer.y.writePtr++ = REG_TO_RAWDATA(data[ACCEL_Y_DATA_INDEX], data[ACCEL_Y_DATA_INDEX + 1]);
    *buffer.z.writePtr++ = REG_TO_RAWDATA(data[ACCEL_Z_DATA_INDEX], data[ACCEL_Z_DATA_INDEX + 1]);
    buffer.samples++;
    return;
}

/**
 * \brief function to handle/act on the latest processed data
 */
void ACCEL_onDataUpdate( void )
{
    if ( controller.print )
    {
        DEBUG_print("%d %6.4f %6.4f %6.4f\r\n", HAL_GetTick(), *buffer.x.readPtr, *buffer.y.readPtr, *buffer.z.readPtr );
    }

    if ( controller.ledControl )
    {
        /* super clunky implementation for now. TODO: CLEANIT */
        if ( *buffer.x.readPtr <= -ACCEL_LED_THRESHOLD )
        {
            GPIO_setLED( GPIO_LED_GREEN, GPIO_LED_ON );
        }
        else
        {
            GPIO_setLED( GPIO_LED_GREEN, GPIO_LED_OFF );
        }
        if ( *buffer.x.readPtr >= ACCEL_LED_THRESHOLD )
        {
            GPIO_setLED( GPIO_LED_RED, GPIO_LED_ON );
        }
        else
        {
            GPIO_setLED( GPIO_LED_RED, GPIO_LED_OFF );
        }
        if ( *buffer.y.readPtr <= -ACCEL_LED_THRESHOLD )
        {
            GPIO_setLED( GPIO_LED_BLUE, GPIO_LED_ON );
        }
        else
        {
            GPIO_setLED( GPIO_LED_BLUE, GPIO_LED_OFF );
        }
        if ( *buffer.y.readPtr >= ACCEL_LED_THRESHOLD )
        {
            GPIO_setLED( GPIO_LED_ORANGE, GPIO_LED_ON );
        }
        else
        {
            GPIO_setLED( GPIO_LED_ORANGE, GPIO_LED_OFF );
        }
    }

    /* increment the read pointers */
    buffer.x.readPtr++;
    buffer.y.readPtr++;
    buffer.z.readPtr++;
    return;
}

/**
 * \brief event handler to process a raw data buffer into filtered data
 */
void ACCEL_processDataBuffer( void )
{
    /* the current buffer bank is the write/read, so we can process the old bank */
    float *xOut = ( buffer.activeBank == ACCEL_BUFF_BANK_0 ) ? buffer.x.bank_1 : buffer.x.bank_0;
    float *yOut = ( buffer.activeBank == ACCEL_BUFF_BANK_0 ) ? buffer.y.bank_1 : buffer.y.bank_0;
    float *zOut = ( buffer.activeBank == ACCEL_BUFF_BANK_0 ) ? buffer.z.bank_1 : buffer.z.bank_0;
    float *xIn  = ( buffer.activeBank == ACCEL_BUFF_BANK_0 ) ? buffer.x.rawBank_1 : buffer.x.rawBank_0;
    float *yIn  = ( buffer.activeBank == ACCEL_BUFF_BANK_0 ) ? buffer.y.rawBank_1 : buffer.y.rawBank_0;
    float *zIn  = ( buffer.activeBank == ACCEL_BUFF_BANK_0 ) ? buffer.z.rawBank_1 : buffer.z.rawBank_0;
    /* filter the signals into the processed data buffer */
    arm_fir_f32( &xFilter, xIn ,xOut, ACCEL_BUFFER_SIZE);
    arm_fir_f32( &yFilter, yIn ,yOut, ACCEL_BUFFER_SIZE);
    arm_fir_f32( &zFilter, zIn ,zOut, ACCEL_BUFFER_SIZE);
    return;
}

/**
 * \brief data ready callback interrupt from the device
 */
void EXTI0_IRQHandler( void )
{
    ACCEL_readData( );
    if ( buffer.samples == ACCEL_BUFFER_SIZE )
    {
        /* flip the bank */
        buffer.activeBank = ( buffer.activeBank == ACCEL_BUFF_BANK_0 ) ? ACCEL_BUFF_BANK_1 : ACCEL_BUFF_BANK_0;
        /* reset the write pointers */
        buffer.x.writePtr = ( buffer.activeBank == ACCEL_BUFF_BANK_0 ) ? buffer.x.rawBank_0 : buffer.x.rawBank_1;
        buffer.y.writePtr = ( buffer.activeBank == ACCEL_BUFF_BANK_0 ) ? buffer.y.rawBank_0 : buffer.y.rawBank_1;
        buffer.z.writePtr = ( buffer.activeBank == ACCEL_BUFF_BANK_0 ) ? buffer.z.rawBank_0 : buffer.z.rawBank_1;

        /* reset the read pointers */
        buffer.x.readPtr =  ( buffer.activeBank == ACCEL_BUFF_BANK_0 ) ? buffer.x.bank_0    : buffer.x.bank_1;
        buffer.y.readPtr =  ( buffer.activeBank == ACCEL_BUFF_BANK_0 ) ? buffer.y.bank_0    : buffer.y.bank_1;
        buffer.z.readPtr =  ( buffer.activeBank == ACCEL_BUFF_BANK_0 ) ? buffer.z.bank_0    : buffer.z.bank_1;
        /* reset the sample counter */
        buffer.samples = 0;

        /* set the event to process the data */
        EVENT_set( &mainEventFlags, EVENT_ACCEL_BUFF_FULL );
    }
    /* act on the data */
    ACCEL_onDataUpdate( );

    /* clear the interrupt */
    __HAL_GPIO_EXTI_CLEAR_IT(ACCEL_DR_INT_PIN);
    return;
}
