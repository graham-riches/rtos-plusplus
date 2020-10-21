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
typedef enum
{
    ACCEL_CONTROL_MODE_LED = 0,
    ACCEL_CONTROL_MODE_NONE,
    ACCEL_CONTROL_TOTAL_MODES
} ACCEL_controlMode_t;


/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/


/****************************** Functions Prototype ************************************/
void ACCEL_init( void );
void ACCEL_test( void );
void ACCEL_printData( void );
void ACCEL_processDataBuffer( void );
void ACCEL_setControlMode( ACCEL_controlMode_t mode );

#ifdef __cplusplus
}
#endif

#endif /* __ACCELEROMETER_H__ */
