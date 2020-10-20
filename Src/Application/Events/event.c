/*! \file event.c
*
*  \brief events module for synchronizing activities across the system.
*
*
*  \author Graham Riches
*/


/********************************** Includes *******************************************/
#include "event.h"
#include "accelerometer.h"
#include "audioin.h"
#include "command_line.h"

/*********************************** Consts ********************************************/


/************************************ Types ********************************************/


/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/
volatile uint32_t mainEventFlags;
EVENT_handler_t mainEventHandler;
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

   /* assign the function pointers for the event group */
   //mainEventHandler.cbs[EVENT_USART_DEBUG_RX] = (EVENT_cb_t)&CLI_mainEventFunc;
   //mainEventHandler.cbs[EVENT_ACCEL_BUFF_FULL] = (EVENT_cb_t)&ACCEL_processDataBuffer;
   //mainEventHandler.cbs[EVENT_AUDIO_IN_BUFF_FULL] = (EVENT_cb_t)&AUDIOIN_processDataBuffer;
   return;
}


/**
 * \brief set/clear an event flag bit for a specific event flags group
 * \param eventFlagGroup the event flags group
 * \param bit the bit to set
 */
void EVENT_set( volatile uint32_t *eventFlagGroup, uint8_t bit )
{
    //DISABLE_INTERRUPTS();
    *eventFlagGroup |= (0x01U << bit);
    //ENABLE_INTERRUPTS();
    return;
}


/**
 * \brief check a specific event flag to see if it has been raised
 * \param eventFlagGroup the event flag group
 * \bit the bit to check
 */
bool EVENT_get( volatile uint32_t *eventFlagGroup, uint8_t bit )
{
   //DISABLE_INTERRUPTS();
   bool event = ( *eventFlagGroup & (0x01U << bit) );
   /* clear the bit */
   if ( event )
   {
      *eventFlagGroup &= ~(0x01U << bit);
   }
   //ENABLE_INTERRUPTS();
   return event;
}


/**
 * \brief call an event callback function for an event group
 * \param handler the container of callback function pointers
 * \param event the event index to call
 */
void EVENT_call( EVENT_handler_t *handler, uint8_t event )
{
   /* call the function pointer */
   handler->cbs[event]();
}
