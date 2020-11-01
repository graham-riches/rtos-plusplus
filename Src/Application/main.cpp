/*! \file main.c
*
*  \brief Main application code
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "board.h"
#include "common.h"
#include "event.h"
#include "gpio.h"
#include "peripherals.h"
#include <string.h>

/*********************************** Consts ********************************************/

/************************************ Types ********************************************/

/*********************************** Macros ********************************************/

/*********************************** Local Variables ********************************************/

/**
  * \brief  Main application function
  * \retval int
  */
int main( void )
{
   /* configure the project specific HAL drivers and bootup the chip */
   PERIPHERAL_moduleInitialize( );

   /* initialize the events module */
   EVENT_init( );

   /* main application loop */
   while ( 1 )
   {
      /* check for events */
      if ( mainEventFlags )
      {
         for ( uint8_t event = 0; event < EVENT_TOTAL_EVENTS; event++ )
         {
            if ( EVENT_get( &mainEventFlags, event ) )
            {
               EVENT_call( &mainEventHandler, event );
            }
         }
      }
   }

   return 0;
}
