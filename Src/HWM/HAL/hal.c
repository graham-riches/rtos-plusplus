/*! \file hal.c
*
*  \brief HAL setup and abstraction.
*
*
*  \author Graham Riches
*/


/********************************** Includes *******************************************/
#include "hal.h"
#include "system.h"
#include "spi.h"
#include "i2c.h"
#include "i2s.h"
#include "gpio.h"
#include "usart.h"

/*********************************** Consts ********************************************/


/************************************ Types ********************************************/


/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/


/******************************** Local Variables **************************************/


/****************************** Functions Prototype ************************************/


/****************************** Functions Definition ***********************************/
/**
* \name     HAL_init
* \brief    Initialize the module variables and resources.
*
* \param    None
* \retval   None
*/
void HAL_moduleInit( void )
{
	/* Configure the system clock */
	SYSTEM_init();

	/* Initialize all configured peripherals */
	SPI_init();
	I2C_init();
	I2S_init();
	GPIO_init();
	USART_init();
}
