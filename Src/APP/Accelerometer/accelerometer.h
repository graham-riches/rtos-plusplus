/*! \file accelerometer.h
*
*  \brief accelerometer module functions and variables declarations.
*
*
*  \author Graham Riches
*  \copyright Copyright (c) ATTAbotics Inc. 2020
*/

#ifndef __ACCELEROMETER_H__
#define __ACCELEROMETER_H__

#ifdef __cplusplus
extern "C" {
#endif
/********************************** Includes *******************************************/
#include "common.h"

/*********************************** Consts ********************************************/
/* LIS302DL API definitions */
#define ACCEL_WRITE       0x00U //!< write bit
#define ACCEL_READ        0x80U //!< read bit

/* Register Addresses */
#define ACCEL_WHO_AM_I  0x0FU //!< device ID register
#define ACCEL_CTRL_REG4 0x20U //!< control register 4




/************************************ Types ********************************************/


/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/


/****************************** Functions Prototype ************************************/
void ACCEL_init( void );
void ACCEL_test( void );


#ifdef __cplusplus
}
#endif

#endif /* __ACCELEROMETER_H__ */
