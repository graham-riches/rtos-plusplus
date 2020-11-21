/*! \file timer.c
*
*  \brief OS timer module for various timing functions.
*
*
*  \author Graham Riches
*/


/********************************** Includes *******************************************/
#include "timer.h"

namespace OS
{
/*********************************** Consts ********************************************/


/************************************ Types ********************************************/


/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/
volatile uint32_t system_ticks;

/******************************** Local Variables **************************************/


/****************************** Functions Prototype ************************************/


/****************************** Functions Definition ***********************************/
/**
 * \brief delay for a set period in milliseconds
 * 
 * \param delay time to delay for
 */
void delay_ms( uint32_t delay )
{
   uint32_t start_tick = system_ticks;
   while ((system_ticks - start_tick) < delay )
   {

   }
}

};
