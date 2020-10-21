/*! \file i2c.h
*
*  \brief i2c module functions and variables declarations.
*
*
*  \author Graham Riches
*/

#ifndef __I2C_H__
#define __I2C_H__

#ifdef __cplusplus
extern "C" {
#endif
/********************************** Includes *******************************************/
#include "common.h"
#include "stm32f4xx_hal.h"

/*********************************** Consts ********************************************/


/************************************ Types ********************************************/
/**
 * \brief handler structure for different I2C peripherals
 */
typedef struct
{
    uint8_t id;
    I2C_HandleTypeDef handler;
} I2C_handler_t;

/**
 * \brief list of I2C peripherals
 */
typedef enum
{
    I2C_DEFAULT_DEVICE = 0,
    I2C_MAX_DEVICES
} I2C_devices_t;

/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/


/****************************** Functions Prototype ************************************/
void I2C_init( void );


#ifdef __cplusplus
}
#endif

#endif /* __I2C_H__ */
