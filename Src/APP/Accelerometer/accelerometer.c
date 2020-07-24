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

/*********************************** Consts ********************************************/
#define ACCEL_2G_SCALE_FACTOR   0x4000
#define G_TO_METER_PER_S2       9.81
#define ACCEL_BUFFER_SIZE       25

/************************************ Types ********************************************/
/**
 * \brief handler structure for processing accelerometer data in banks
 */
typedef struct
{
    ACCEL_data_t bank_0[ACCEL_BUFFER_SIZE]; //!< first buffer bank
    ACCEL_data_t bank_1[ACCEL_BUFFER_SIZE]; //!< second buffer bank
    ACCEL_data_t *writePtr;                 //!< current write pointer
    ACCEL_data_t *readPtr;                  //!< read pointer
    uint8_t samples;                        //!< samples read
    uint8_t activeBank;                     //!< active bank index
} ACCEL_bufferHandler_t;

/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/


/******************************** Local Variables **************************************/
static ACCEL_bufferHandler_t buffer = {0};


/****************************** Functions Prototype ************************************/
void ACCEL_write( uint8_t addr, uint8_t command );
void ACCEL_readData( ACCEL_data_t *data );

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
    ACCEL_write( ACCEL_CTRL_REG4, ACCEL_WAKE_UP );

    /* enable the data ready interrupt */
    ACCEL_write( ACCEL_CTRL_REG3, ACCEL_ENABLE_DR_INT );

    /* enable multi-byte reading */
    ACCEL_write( ACCEL_CTRL_REG6, ACCEL_ENABLE_MULTIBYTE_READ );

    /* set the data processing buffer pointer to the first bank */
    buffer.writePtr = buffer.bank_0;

    /* set the print pointer the first instance of processed data */
    buffer.readPtr = buffer.bank_1;

    buffer.activeBank = 0;
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
 * \brief read the raw accelerometer data back from the device
 * \param data pointer to a raw data container
 * \NOTE the device registers are all sequential so it is faster
 *       to read them all in a chain. At this point, there is
 *       really no escaping the magic numbers in this function
 */
void ACCEL_readData( ACCEL_data_t *rawData )
{
    /* read the device data registers */
    uint8_t readCmd[7] = {0};
    uint8_t data[7] = {0};

    /* read the registers */
    readCmd[0] = (ACCEL_READ | ACCEL_OUT_X_L );
    SPI_readWrite( SPI_ACCELEROMETER, (uint8_t *)&readCmd, (uint8_t *)&data, sizeof(readCmd) );
    rawData->x = (float)((int16_t)((uint16_t)data[1]  | ((uint16_t)data[2] << 8)))/ACCEL_2G_SCALE_FACTOR;
    rawData->y = (float)((int16_t)((uint16_t)data[3]  | ((uint16_t)data[4] << 8)))/ACCEL_2G_SCALE_FACTOR;
    rawData->z = (float)((int16_t)((uint16_t)data[6]  | ((uint16_t)data[6] << 8)))/ACCEL_2G_SCALE_FACTOR;

    return;
}

/**
 * \brief print out the current data
 * \param data pointer to data to print
 */
void ACCEL_printData( ACCEL_data_t *data )
{
    DEBUG_print("%d %6.4f %6.4f %6.4f\n", HAL_GetTick(), data->x, data->y, data->z );
    return;
}

/**
 * \brief data ready callback interrupt from the device
 */
void EXTI0_IRQHandler( void )
{
    ACCEL_readData( buffer.writePtr++ );
    buffer.samples++;
    if ( buffer.samples == ACCEL_BUFFER_SIZE -1 )
    {
        /* flip the bank */
        buffer.activeBank = ( buffer.activeBank == 0 ) ? 1 : 0;
        buffer.writePtr = ( buffer.activeBank == 0 ) ? buffer.bank_0 : buffer.bank_1;
        buffer.readPtr = ( buffer.activeBank == 0 ) ? buffer.bank_1 : buffer.bank_0;
        buffer.samples = 0;
    }
    /* print out the filtered data */
    ACCEL_printData( buffer.readPtr++ );

    /* clear the interrupt */
    __HAL_GPIO_EXTI_CLEAR_IT(ACCEL_DR_INT_PIN);
    return;
}
