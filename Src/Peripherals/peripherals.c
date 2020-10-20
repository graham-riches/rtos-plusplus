/*! \file peripherals.c
*
*  \brief HAL setup and abstraction for all peripherals
*
*
*  \author Graham Riches
*/


/********************************** Includes *******************************************/
#include "peripherals.h"
#include "system.h"
#include "gpio.h"
//#include "spi.h"
//#include "i2c.h"
//#include "i2s.h"
//#include "usart.h"
//#include "adc.h"
//#include "dma.h"

/*********************************** Consts ********************************************/


/************************************ Types ********************************************/


/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/


/******************************** Local Variables **************************************/


/****************************** Functions Prototype ************************************/


/****************************** Functions Definition ***********************************/
/**
* \name     PERIPHERAL_moculdeInitialize
* \brief    Initialize the module variables and resources.
*
* \param    None
* \retval   None
*/
void PERIPHERAL_moduleInitialize( void )
{
	/* Configure the system clock */
	SYSTEM_init();

	/* Initialize all configured peripherals */
	GPIO_init();

}
