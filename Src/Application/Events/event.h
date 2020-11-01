/*! \file event.h
*
*  \brief event module functions and variables declarations.
*
*
*  \author Graham Riches
*/

#ifndef __EVENT_H__
#define __EVENT_H__

#ifdef __cplusplus
extern "C" {
#endif
/********************************** Includes *******************************************/
#include "common.h"

/*********************************** Consts ********************************************/
#define MAX_EVENT_GROUP_SIZE 32 //!< max number of events in a group is 32 (using 32-bit variable for flags)

/************************************ Types ********************************************/
/**
 * \brief enumeration of system events bits
 */
typedef enum
{
    EVENT_USART_DEBUG_RX = 0,     //!< received data on UART
    EVENT_TICK_UPDATE,            //!< got a tick update from the systick timer
    EVENT_TOTAL_EVENTS
} EVENT_mainEvents_t;

/**
 * \brief function pointer for event handling callbacks
 */
typedef void (*EVENT_cb_t)(void);

/**
 * \brief handler for storing function callbacks for an event group
 */
typedef struct
{
    EVENT_cb_t cbs[MAX_EVENT_GROUP_SIZE];
} EVENT_handler_t;

/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/
extern volatile uint32_t mainEventFlags;
extern EVENT_handler_t mainEventHandler;

/****************************** Functions Prototype ************************************/
void EVENT_init( void );
void EVENT_set( volatile uint32_t *eventFlagGroup, uint8_t bit );
bool EVENT_get( volatile uint32_t *eventFlagGroup, uint8_t bit );
void EVENT_call( EVENT_handler_t *handler, uint8_t event );

#ifdef __cplusplus
}
#endif

#endif /* __EVENT_H__ */
