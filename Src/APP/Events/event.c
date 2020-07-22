/*! \file event.c
*
*  \brief events module for synchronizing activities across the system.
*
*
*  \author Graham Riches
*  \copyright Copyright (c) ATTAbotics Inc. 2020
*/


/********************************** Includes *******************************************/
#include "event.h"

/*********************************** Consts ********************************************/


/************************************ Types ********************************************/


/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/
volatile uint32_t mainEventFlags;

/******************************** Local Variables **************************************/


/****************************** Functions Prototype ************************************/


/****************************** Functions Definition ***********************************/
/**
* \name     EVENT_init
* \brief    Initialize the module variables and resources.
*
* \param    None
* \retval   None
*/
void EVENT_init( void )
{
    /* clear the event flag groups */
   mainEventFlags = 0;
   return;
}


/**
 * \brief set/clear an event flag bit for a specific event flags group
 * \param eventFlagGroup the event flags group
 * \param bit the bit to set
 */
void EVENT_set( volatile uint32_t *eventFlagGroup, uint8_t bit )
{
    DISABLE_INTERRUPTS();
    *eventFlagGroup |= (0x01U << bit);
    ENABLE_INTERRUPTS();
    return;
}

/**
 * \brief check a specific event flag to see if it has been raised
 * \param eventFlagGroup the event flag group
 * \bit the bit to check
 */
bool EVENT_get( volatile uint32_t *eventFlagGroup, uint8_t bit )
{
    DISABLE_INTERRUPTS();
    bool event = ( *eventFlagGroup & (0x01U << bit) );
    /* clear the bit */
    if ( event )
    {
        *eventFlagGroup &= ~(0x01U << bit);
    }
    ENABLE_INTERRUPTS();
    return event;
}
