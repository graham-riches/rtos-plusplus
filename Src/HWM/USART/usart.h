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
#include "common.h"
#include "stm32f4xx_hal.h"

/*********************************** Consts ********************************************/
#define USART_BAUD_115200 115200

/************************************ Types ********************************************/

/**
 * \brief handler structure for USART peripherals
 */
typedef struct
{
    uint8_t id;
    UART_HandleTypeDef handler;
} USART_handler_t;

/**
 * \brief enumeration of USART peripherals
 */
typedef enum
{
    USART_DEBUG = 0, //!< debug port (USART3)
    USART_TOTAL_DEVICES
} USART_devices_t;

/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/


/****************************** Functions Prototype ************************************/
void USART_init( void );
void USART_send( USART_devices_t device, uint8_t *data, uint16_t size );


#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */
