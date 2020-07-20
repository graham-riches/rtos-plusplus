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
#define ACCEL_READ        0x01U //!< read bit
#define ACCEL_INC_ADDRESS 0x02U //!< increment the address for successive readings
#define ACCEL_ADDR_OFFSET 2 //!< address is bit-shifted left two from the start of the packet

/* Register Addresses */
#define ACCEL_WHO_AM_I 0x0FU //!< device ID register


/* 0b<D0><D1><D2><D3><D4><D5><D6><D7><AD0><AD1><AD2><AD3><AD4><AD5><MS><RW> */

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
