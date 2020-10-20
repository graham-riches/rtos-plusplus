/*! \file debug.h
*
*  \brief debug module functions and variables declarations.
*
*  \author Graham Riches
*/

#ifndef __DEBUG_H__
#define __DEBUG_H__

#ifdef __cplusplus
extern "C" {
#endif
/********************************** Includes *******************************************/
#include "common.h"

/*********************************** Consts ********************************************/


/************************************ Types ********************************************/


/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/


/****************************** Functions Prototype ************************************/
void DEBUG_init( void );
void DEBUG_log( const char *msg, ... );
void DEBUG_print( const char *msg, ... );


#ifdef __cplusplus
}
#endif

#endif /* __DEBUG_H__ */
