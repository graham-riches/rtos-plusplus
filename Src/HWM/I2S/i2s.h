/*! \file i2s.h
*
*  \brief i2s module functions and variables declarations.
*
*
*  \author Graham Riches
*/

#ifndef __I2S_H__
#define __I2S_H__

#ifdef __cplusplus
extern "C" {
#endif
/********************************** Includes *******************************************/
#include "common.h"
#include "stm32f4xx_hal.h"

/*********************************** Consts ********************************************/


/************************************ Types ********************************************/

/**
 * \brief handler structure for I2S peripherals
 */
typedef struct
{
    uint8_t id;
    I2S_HandleTypeDef handler;
} I2S_handler_t;


/**
 * \brief list of all I2S devices
 */
typedef enum
{
    I2S_AUDIO_DEVICE = 0,
    I2S_MAX_DEVICES
} I2S_devices_t;

/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/


/****************************** Functions Prototype ************************************/
void I2S_init( void );


#ifdef __cplusplus
}
#endif

#endif /* __I2S_H__ */
