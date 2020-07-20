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
   /* Initialize the module here*/

}


/**
 * \brief read the devices WHO_AM_I register as a test
 *
 */
void ACCEL_test( void )
{
    uint16_t data = 0;
    ACCEL_read( ACCEL_WHO_AM_I, &data, sizeof(data) );
    return;
}



/**
 * \brief read from the device
 */
void ACCEL_read( uint16_t addr, uint8_t *msg, uint16_t size )
{
    /* assemble the read instruction */
    uint16_t readMsg = 0;
    readMsg |= ACCEL_READ; //!< enable the read bit
    readMsg |= (addr << ACCEL_ADDR_OFFSET); //!< pass in the address
    SPI_write( SPI_ACCELEROMETER, &readMsg, sizeof(readMsg) );
    /* wait for the data to come back */
    SPI_read( SPI_ACCELEROMETER, msg, size );
    return;
}

