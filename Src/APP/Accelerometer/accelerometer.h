/*! \file accelerometer.h
*
*  \brief accelerometer module functions and variables declarations.
*
*
*  \author Graham Riches
*/

#ifndef __ACCELEROMETER_H__
#define __ACCELEROMETER_H__

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
void ACCEL_init( void );
void ACCEL_test( void );
void ACCEL_printData( void );
void ACCEL_processDataBuffer( void );

#ifdef __cplusplus
}
#endif

#endif /* __ACCELEROMETER_H__ */
