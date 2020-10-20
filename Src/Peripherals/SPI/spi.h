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
#include "stm32f4xx.h"
#include "stm32f4xx_spi.h"

/*********************************** Consts ********************************************/


/************************************ Types ********************************************/
/**
 * \brief handler structure for different SPI peripherals
 */
typedef struct
{
	uint8_t cs;                 //!< gpio pin of the CS pin
	GPIO_TypeDef *bank;         //!< gpio bank of the SPI CS pin
	SPI_InitTypeDef handler;	//!< HAL SPI handler type
} SPI_handler_t;


/**
 * \brief different SPI peripherals
 */
typedef enum
{
	SPI_ACCELEROMETER = 0,
	SPI_TOTAL_DEVICES
} SPI_devices_t;



/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/


/****************************** Functions Prototype ************************************/
void SPI_init( void );
void SPI_readWrite( SPI_devices_t device, uint8_t *txBuff, uint8_t *rxBuff, uint16_t size );
void SPI_write( SPI_devices_t device, uint8_t *txBuff, uint16_t size );
void SPI_read( SPI_devices_t device, uint8_t *rxBuff, uint16_t size );


#ifdef __cplusplus
}
#endif

#endif /* __SPI_H__ */
