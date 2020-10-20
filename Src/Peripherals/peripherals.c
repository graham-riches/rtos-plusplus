/*! \file peripherals.c
*
*  \brief HAL setup and abstraction for all peripherals
*
*
*  \author Graham Riches
*/


/********************************** Includes *******************************************/
#include "peripherals.h"
#include "rcc.h"
#include "gpio.h"

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
	/* Configure the system clocks */
	RCC_initialize();

	/* Initialize all configured peripherals */
	GPIO_init();

}
