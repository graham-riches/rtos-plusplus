/*! \file command_line.h
 *	 \brief command line module for RCP development
 *  \details
 *	    This header contains the types and functions used in the menu interface.
 *
 *	 \author Graham Riches
 */

#ifndef _COMMAND_LINE_H
#define _COMMAND_LINE_H

#ifdef __cplusplus
extern "C"
{
#endif

/********************************** Includes *******************************************/
#include "common.h"
#include <stdint.h>

/********************************** Defines *******************************************/
#define CLI_RX_BUFFER_SIZE 512

#define CLI_MAX_COMMANDS 10
#define CLI_MAX_LINE_LEN 128
#define CLI_MAX_ARG_LEN  128
#define CLI_MAX_ARGS     10


/******************************* Global Variables **************************************/


/************************************ Types ********************************************/

/**
*	\brief contains callback and information for a single command.
*		   Each command must have one of these structs.
*	*callback --> function pointer to a callback function
*  *name     --> the text to be input at the command line to initaie the command
*  *usage    --> help menu usage display.
*/
typedef struct
{
   int                ( *callback )( int, char *[] );
   const char         *name;
   const char * const *usage;
   const char         *description;
}CLI_command_t;


/****************************** Functions Prototype ************************************/

bool CLI_init( const CLI_command_t **commandList );
int32_t  CLI_executeCommand( char *commandString );
void CLI_listCommands( void );


#ifdef __cplusplus
} //extern "C"
#endif

#endif //_COMMAND_LINE_H
