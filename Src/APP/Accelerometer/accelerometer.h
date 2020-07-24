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
#define ACCEL_WRITE                 0x00U //!< write bit
#define ACCEL_READ                  0x80U //!< read bit
#define ACCEL_WAKE_UP               0x67U //!< wake-up command
#define ACCEL_ENABLE_DR_INT         0xC8U //!< enable data ready interrupt
#define ACCEL_ENABLE_MULTIBYTE_READ 0x10U //!< enable multi-byte read chaining
#define ACCEL_INFO                  0x0DU


/* Register Addresses */
#define ACCEL_WHO_AM_I  0x0FU //!< device ID register
#define ACCEL_CTRL_REG1 0x21U //!< control register 1
#define ACCEL_CTRL_REG2 0x22U //!< control register 2
#define ACCEL_CTRL_REG3 0x23U //!< control register 3
#define ACCEL_CTRL_REG4 0x20U //!< control register 4
#define ACCEL_CTRL_REG5 0x24U //!< control register 5
#define ACCEL_CTRL_REG6 0x25U //!< control register 6
#define ACCEL_SR_REG    0x27U //!< device status register
#define ACCEL_OUT_X_L   0x28U //!< acceleration output low-byte in X-axis
#define ACCEL_OUT_X_H   0x29U //!< acceleration output low-byte in X-axis
#define ACCEL_OUT_Y_L   0x2AU //!< acceleration output low-byte in Y-axis
#define ACCEL_OUT_Y_H   0x2BU //!< acceleration output low-byte in Y-axis
#define ACCEL_OUT_Z_L   0x2CU //!< acceleration output low-byte in Z-axis
#define ACCEL_OUT_Z_H   0x2DU //!< acceleration output low-byte in Z-axis



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
