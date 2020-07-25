/*! \file accelerometer.c
*
*  \brief contains the implementation of the LIS302DL 3-Axis MEMS accelerometer driver.
*
*
*  \author Graham Riches
*  \copyright Copyright (c) ATTAbotics Inc. 2020
*/


/********************************** Includes *******************************************/
#include "accelerometer.h"
#include "stm32f4xx_hal.h"
#include "spi.h"
#include "debug.h"
#include "board.h"
#include "event.h"
#include "arm_math.h"
#include <string.h>

/*********************************** Consts ********************************************/
#define ACCEL_2G_SCALE_FACTOR   0x4000  //!< convert from +/- 2g int data to floating point
#define G_TO_METER_PER_S2       9.81    //!< gravitational conversion from [g] to [m/s2]
#define ACCEL_BUFFER_SIZE       64      //!< number of samples per data buffer
#define ACCEL_DSP_FILTER_TAPS   64      //!< arm math filtering


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
    uint8_t samples;    //!< samples read
    uint8_t activeBank; //!< active bank index
} ACCEL_bufferHandler_t;

/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/

/******************************** DSP Filter Parameters **************************************/
float filtStateArr[ACCEL_DSP_FILTER_TAPS + ACCEL_BUFFER_SIZE - 1] = {0};
/* TODO: output these from python in single precision */
float filtCoeffs[ACCEL_DSP_FILTER_TAPS] = {-0.0007951257757810831, -0.0008659828988107479, -0.0009697066797947907, -0.001103324214140007,
                                           -0.0012560822765565855, -0.001409098732298799, -0.0015355381129686498, -0.001601331767186395,
                                           -0.0015664314719828836, -0.00138655346727461, -0.001015339391494171, -0.0004068333459389835,
                                           0.00048184805516356886, 0.0016877918824191557, 0.003239659311786027, 0.005155298963715912,
                                           0.007439744328692877, 0.010083720903916429, 0.013062765798245978, 0.016337033484235645,
                                           0.019851827567365473, 0.02353886167102235, 0.02731821476024491, 0.031100909489785118,
                                           0.03479200847415908, 0.038294094606948256, 0.041510979322065836, 0.044351468267380595,
                                           0.0467330081114202, 0.048585041520485636, 0.04985190962835438, 0.050495161986820296,
                                           0.050495161986820296, 0.049851909628354375, 0.048585041520485636, 0.0467330081114202,
                                           0.044351468267380595, 0.041510979322065836, 0.03829409460694825, 0.03479200847415908,
                                           0.031100909489785115, 0.027318214760244906, 0.023538861671022336, 0.019851827567365473,
                                           0.01633703348423564, 0.013062765798245978, 0.010083720903916425, 0.007439744328692877,
                                           0.00515529896371591, 0.0032396593117860243, 0.001687791882419155, 0.0004818480551635685,
                                           -0.0004068333459389835, -0.0010153393914941703, -0.00138655346727461, -0.0015664314719828825,
                                           -0.0016013317671863929, -0.0015355381129686482, -0.0014090987322987972, -0.0012560822765565855,
                                           -0.001103324214140007, -0.0009697066797947907, -0.0008659828988107479, -0.0007951257757810831 };

arm_fir_instance_f32 filter;
/******************************** Local Variables **************************************/
static ACCEL_bufferHandler_t buffer = {0};

/****************************** Functions Prototype ************************************/
void ACCEL_write( uint8_t addr, uint8_t command );
void ACCEL_readData( void );

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

    /* initialize the filter object */
    arm_fir_init_f32( &filter, ACCEL_DSP_FILTER_TAPS, filtCoeffs, filtStateArr, ACCEL_BUFFER_SIZE);

    return;
}


/**
 * \brief read the devices WHO_AM_I register as a test
 *
 */
void ACCEL_test( void )
{
    uint16_t command = (ACCEL_WHO_AM_I | ACCEL_READ);
    uint16_t data = 0;
    SPI_readWrite( SPI_ACCELEROMETER, (uint8_t *)&command, (uint8_t *)&data, sizeof(command) );
    /* NOTE: the data we are interested in is in the second received byte */
    DEBUG_print("ACCELEROMETER TEST: received: %d\n", (uint8_t)(data >> 8));
    return;
}


/**
 * \brief write to a data register on the device
 * \param addr the register to write to
 * \command the data to write to the register
 */
void ACCEL_write( uint8_t addr, uint8_t command )
{
    uint16_t payload = (command << 8 ) | (addr | ACCEL_WRITE);
    SPI_write( SPI_ACCELEROMETER, (uint8_t *)&payload, sizeof(payload) );
}


/**
 * \brief read the raw accelerometer data back from the device into the storage buffers
 * \NOTE the device registers are all sequential so it is faster
 *       to read them all in a chain. At this point, there is
 *       really no escaping the magic numbers in this function
 */
void ACCEL_readData( void )
{
    /* read the device data registers */
    uint8_t readCmd[7] = {0};
    uint8_t data[7] = {0};

    /* read the registers */
    readCmd[0] = (ACCEL_READ | ACCEL_OUT_X_L );
    SPI_readWrite( SPI_ACCELEROMETER, (uint8_t *)&readCmd, (uint8_t *)&data, sizeof(readCmd) );
    *buffer.x.writePtr++ = (float)((int16_t)((uint16_t)data[1]  | ((uint16_t)data[2] << 8)))/ACCEL_2G_SCALE_FACTOR;
    *buffer.y.writePtr++ = (float)((int16_t)((uint16_t)data[3]  | ((uint16_t)data[4] << 8)))/ACCEL_2G_SCALE_FACTOR;
    *buffer.z.writePtr++ = (float)((int16_t)((uint16_t)data[6]  | ((uint16_t)data[6] << 8)))/ACCEL_2G_SCALE_FACTOR;
    buffer.samples++;
    return;
}

/**
 * \brief print out the current data
 * \param data pointer to data to print
 */
void ACCEL_printData( void )
{
    DEBUG_print("%d %6.4f %6.4f %6.4f\r\n", HAL_GetTick(), *buffer.x.readPtr++, *buffer.y.readPtr++, *buffer.z.readPtr++ );
    return;
}

/**
 * \brief event handler to process a raw data buffer into filtered data
 */
void ACCEL_processDataBuffer( void )
{
    /* the current buffer bank is the write/read, so we can process the old bank */
    float *xOut = ( buffer.activeBank == 0 ) ? buffer.x.bank_1 : buffer.x.bank_0;
    float *yOut = ( buffer.activeBank == 0 ) ? buffer.y.bank_1 : buffer.y.bank_0;
    float *zOut = ( buffer.activeBank == 0 ) ? buffer.z.bank_1 : buffer.z.bank_0;
    float *xIn  = ( buffer.activeBank == 0 ) ? buffer.x.rawBank_1 : buffer.x.rawBank_0;
    float *yIn  = ( buffer.activeBank == 0 ) ? buffer.y.rawBank_1 : buffer.y.rawBank_0;
    float *zIn  = ( buffer.activeBank == 0 ) ? buffer.z.rawBank_1 : buffer.z.rawBank_0;
    /* filter the signals into the processed data buffer */
    arm_fir_f32( &filter, xIn ,xOut, ACCEL_BUFFER_SIZE);
    arm_fir_f32( &filter, yIn ,yOut, ACCEL_BUFFER_SIZE);
    arm_fir_f32( &filter, zIn ,zOut, ACCEL_BUFFER_SIZE);

    /*
    memcpy( xOut, xIn, sizeof(buffer.x.bank_0) );
    memcpy( yOut, yIn, sizeof(buffer.y.bank_0) );
    memcpy( zOut, zIn, sizeof(buffer.z.bank_0) );
    */
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
        buffer.activeBank = ( buffer.activeBank == 0 ) ? 1 : 0;
        /* reset the pointers */
        buffer.x.writePtr = ( buffer.activeBank == 0 ) ? buffer.x.rawBank_0 : buffer.x.rawBank_1;
        buffer.x.readPtr =  ( buffer.activeBank == 0 ) ? buffer.x.bank_0    : buffer.x.bank_1;
        buffer.y.writePtr = ( buffer.activeBank == 0 ) ? buffer.y.rawBank_0 : buffer.y.rawBank_1;
        buffer.y.readPtr =  ( buffer.activeBank == 0 ) ? buffer.y.bank_0    : buffer.y.bank_1;
        buffer.z.writePtr = ( buffer.activeBank == 0 ) ? buffer.z.rawBank_0 : buffer.z.rawBank_1;
        buffer.z.readPtr =  ( buffer.activeBank == 0 ) ? buffer.z.bank_0    : buffer.z.bank_1;
        /* reset the sample counter */
        buffer.samples = 0;

        /* set the event to process the data */
        EVENT_set( &mainEventFlags, EVENT_ACCEL_BUFF_FULL );
    }
    /* print out the filtered data */
    ACCEL_printData( );

    /* clear the interrupt */
    __HAL_GPIO_EXTI_CLEAR_IT(ACCEL_DR_INT_PIN);
    return;
}
