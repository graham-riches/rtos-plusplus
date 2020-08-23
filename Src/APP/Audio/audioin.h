/*! \file audioin.h
*
*  \brief audioin module functions and variables declarations.
*
*
*  \author Graham Riches
*/

#ifndef __AUDIOIN_H__
#define __AUDIOIN_H__

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
void AUDIOIN_init( void );
void AUDIOIN_processDataBuffer( void );

#ifdef __cplusplus
}
#endif

#endif /* __AUDIOIN_H__ */
