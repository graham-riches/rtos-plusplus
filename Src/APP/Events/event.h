/*! \file event.h
*
*  \brief event module functions and variables declarations.
*
*
*  \author Graham Riches
*  \copyright Copyright (c) ATTAbotics Inc. 2020
*/

#ifndef __EVENT_H__
#define __EVENT_H__

#ifdef __cplusplus
extern "C" {
#endif
/********************************** Includes *******************************************/
#include "common.h"

/*********************************** Consts ********************************************/


/************************************ Types ********************************************/
/**
 * \brief enumeration of system events bits
 */
typedef enum
{
    EVENT_USART_DEBUG_RX = 0,
    EVENT_ACCEL_BUFF_FULL,
    EVENT_TOTAL_EVENTS
} EVENT_mainEvents_t;

/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/
extern volatile uint32_t mainEventFlags;

/****************************** Functions Prototype ************************************/
void EVENT_init( void );
void EVENT_set( volatile uint32_t *eventFlagGroup, uint8_t bit );
bool EVENT_get( volatile uint32_t *eventFlagGroup, uint8_t bit );

#ifdef __cplusplus
}
#endif

#endif /* __EVENT_H__ */
