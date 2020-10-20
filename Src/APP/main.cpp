/*! \file main.c
*
*  \brief Main application code
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include <string.h>
#include "board.h"
#include "hal.h"
#include "common.h"
#include "gpio.h"
//#include "accelerometer.h"
#include "event.h"


/*********************************** Consts ********************************************/


/************************************ Types ********************************************/


/*********************************** Macros ********************************************/


/*********************************** Local Variables ********************************************/


/**
  * \brief  Main application function
  * \retval int
  */
int main(void)
{
   /* configure the project specific HAL drivers */
   HAL_moduleInit();

   /* initialize the events module */
   EVENT_init();

   /* initialize the accelerometer */
   //ACCEL_init();

   GPIO_setLED( GPIO_LED_GREEN, GPIO_LED_ON );

   /* main application loop */
   while (1)
   {
      /* check for events */
      if ( mainEventFlags )
      {
         for ( uint8_t event = 0; event < EVENT_TOTAL_EVENTS; event ++ )
         {
               if ( EVENT_get(&mainEventFlags, event) )
               {
                  //EVENT_call( &mainEventHandler, event );
               }
         }
      }
   }
   return 0;
}
