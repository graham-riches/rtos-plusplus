/*! \file usart.h
*
*  \brief usart module functions and variables declarations.
*
*
*  \author Graham Riches
*  \copyright Copyright (c) ATTAbotics Inc. 2020
*/

#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif
/********************************** Includes *******************************************/
#include "stm32f4xx_hal.h"
#include "utilities.h"


/*********************************** Consts ********************************************/
#define USART_BAUD_115200 115200

/************************************ Types ********************************************/
/**
 * \brief handler structure for USART peripherals
 */
typedef struct
{
    UART_HandleTypeDef handler;       //!< UART HAL handler
    UTILITIES_ringBuffer_t *rxBuffer; //!< pointer to a rx buffer struct for storing the data
    UTILITIES_ringBuffer_t *txBuffer; //!< pointer to a tx buffer for sending data
} USART_handler_t;

/**
 * \brief enumeration of USART peripherals
 */
typedef enum
{
    USART_DEBUG = 0,     //!< debug port (USART3)
    USART_TOTAL_DEVICES, //!< enumeration count limit

    /* define an invalid value for error handling. Note: this caps the number of devices unless modified */
    USART_INVALID_DEVICE = 0xFF //!< invalid device value
} USART_devices_t;

/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/

/****************************** Functions Prototype ************************************/
void USART_init( void );
void USART_send( USART_devices_t device, uint8_t *data, uint16_t size );
uint16_t USART_recv( USART_devices_t device, uint8_t *data, uint16_t maxSize );




#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */
