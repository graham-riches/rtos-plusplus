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


/****************************** Functions Prototype ************************************/
void ACCEL_read( uint16_t addr, uint8_t *msg, uint16_t size );

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
    uint16_t command = (ACCEL_CTRL_REG4 | ACCEL_WRITE);
    //SPI_write( SPI_ACCELEROMETER, (uint8_t *)&command, sizeof(command) );
    return;
}


/**
 * \brief read the devices WHO_AM_I register as a test
 *
 */
void ACCEL_test( void )
{
    uint16_t command = (ACCEL_WHO_AM_I | ACCEL_READ);
    uint8_t data = SPI_readWrite( SPI_ACCELEROMETER, (uint8_t)command );
    DEBUG_print("ACCELEROMETER TEST: received: %d\n", data);
    return;
}



/**
 * \brief read from the device
 */
void ACCEL_read( uint16_t addr, uint8_t *msg, uint16_t size )
{
    /* assemble the read instruction */
    uint16_t writeMsg = 0;
    writeMsg |= ACCEL_READ; //!< enable the read bit
    writeMsg |= addr; //!< pass in the address
    SPI_write( SPI_ACCELEROMETER, &writeMsg, sizeof(writeMsg) );
    return;
}

