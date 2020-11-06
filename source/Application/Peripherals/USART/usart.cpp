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

   /* print out a splash screen */
   debug_port.send( "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" );
   debug_port.send( "STM32F407 Debug Terminal \n");
   debug_port.send( "   Author: Graham Riches \n");
   debug_port.send( "   Date: November 1, 2020 \n");
   debug_port.send( "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" );
}

