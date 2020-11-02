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

   /* main application loop */
   while ( 1 )
   {

   }

   return 0;
}
