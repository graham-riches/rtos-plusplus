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
#include "spi.h"
#include "debug.h"
#include "board.h"

/*********************************** Consts ********************************************/


/************************************ Types ********************************************/


/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/


/******************************** Local Variables **************************************/
static ACCEL_rawData_t rawData;

/****************************** Functions Prototype ************************************/
void ACCEL_write( uint8_t addr, uint8_t command );
void ACCEL_readData( ACCEL_rawData_t *data );

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
void ACCEL_readData( ACCEL_rawData_t *rawData )
{
    /* read the device data registers */
    uint8_t readCmd[7] = {0};
    uint8_t data[7] = {0};

    /* read the registers */
    readCmd[0] = (ACCEL_READ | ACCEL_OUT_X_L );
    SPI_readWrite( SPI_ACCELEROMETER, (uint8_t *)&readCmd, (uint8_t *)&data, sizeof(readCmd) );
    rawData->x = ((uint16_t)data[1]  | ((uint16_t)data[2] << 8));
    rawData->y = ((uint16_t)data[3]  | ((uint16_t)data[4] << 8));
    rawData->z = ((uint16_t)data[6]  | ((uint16_t)data[6] << 8));

    return;
}

/**
 * \brief debug test function to print out the raw data
 */
void ACCEL_checkData( void )
{
    DEBUG_print("ACCEL [X,Y,Z]: %d %d %d\n", rawData.x, rawData.y, rawData.z );
    return;
}

/**
 * \brief data ready callback interrupt from the device
 */
void EXTI0_IRQHandler( void )
{
    ACCEL_readData( &rawData );
    /* clear the interrupt */
    __HAL_GPIO_EXTI_CLEAR_IT(ACCEL_DR_INT_PIN);
    return;
}
