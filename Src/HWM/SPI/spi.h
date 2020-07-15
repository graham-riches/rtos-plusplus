/*! \file spi.h
*
*  \brief spi module functions and variables declarations.
*
*
*  \author Graham Riches
*/

#ifndef __SPI_H__
#define __SPI_H__

#ifdef __cplusplus
extern "C" {
#endif


/********************************** Includes *******************************************/
#include <stdint.h>
#include "common.h"
#include "stm32f4xx_hal.h"

/*********************************** Consts ********************************************/


/************************************ Types ********************************************/
/**
 * \brief handler structure for different SPI peripherals
 */
typedef struct
{
	uint8_t id;					// identifier for the SPI handler
	SPI_HandleTypeDef handler;	// HAL SPI handler type
} SPI_handler_t;


/**
 * \brief different SPI peripherals
 */
typedef enum
{
	SPI_DEFAULT_DEVICE = 0,
	SPI_MAX_DEVICES
} SPI_devices_t;



/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/


/****************************** Functions Prototype ************************************/
void SPI_init( void );


#ifdef __cplusplus
}
#endif

#endif /* __SPI_H__ */
