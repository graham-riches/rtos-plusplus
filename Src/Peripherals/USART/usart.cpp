/*! \file usart.c
*
*  \brief setup the usart peripherals in the application.
*
*
*  \author Graham Riches
*/


/********************************** Includes *******************************************/
#include "usart.h"
#include "debug_port.h"

/*********************************** Consts ********************************************/


/************************************ Types ********************************************/


/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/


/******************************** Local Variables **************************************/


/****************************** Functions Prototype ************************************/


/****************************** Functions Definition ***********************************/
/**
* \brief    Initialize the module variables and resources.
*
* \param    None
* \retval   None
*/
void USART_initialize( void )
{
   debug_port.initialize();
   debug_port.debug( "this is a test message" );
}

