/*! \file debug.c
*
*  \brief contains the debug message framework.
*
*
*  \author Graham Riches
*  \copyright Copyright (c) ATTAbotics Inc. 2020
*/


/********************************** Includes *******************************************/
#include "debug.h"
#include "usart.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

/*********************************** Consts ********************************************/
#define DEBUG_PRINT_SIZE 256

/************************************ Types ********************************************/


/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/


/******************************** Local Variables **************************************/
static char printBuffer[DEBUG_PRINT_SIZE] = {0};

/****************************** Functions Prototype ************************************/


/****************************** Functions Definition ***********************************/
/**
* \name     DEBUG_init
* \brief    Initialize the module variables and resources.
*
* \param    None
* \retval   None
*/
void DEBUG_init( void )
{
   /* Initialize the module here*/

}



/**
 * \brief debug logging function to print out messages to the terminal
 */
void DEBUG_log( const char *msg, ... )
{
    return;
}

/**
 * \brief print function for USART debugging
 */
void DEBUG_print( const char *msg, ... )
{
    va_list(args);
    va_start( args, msg );
    vsprintf( &printBuffer, msg, args );
    va_end( args );
    USART_send( USART_DEBUG, &printBuffer, strnlen(printBuffer, DEBUG_PRINT_SIZE) );
}


