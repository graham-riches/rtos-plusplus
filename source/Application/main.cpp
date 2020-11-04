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
#include "threading.h"
#include "scheduler.h"
#include <string.h>

/*********************************** Consts ********************************************/
#define THREAD_STACK_SIZE 128


/************************************ Types ********************************************/

/*********************************** Macros ********************************************/

/*********************************** Local Functions ********************************************/
static void thread_one_task( void );
static void thread_two_task( void );


/*********************************** Local Variables ********************************************/
/* create the thread stacks */
static uint32_t thread_one_stack[THREAD_STACK_SIZE] = { 0 };
static uint32_t thread_two_stack[THREAD_STACK_SIZE] = { 0 };

/* create the threads */
static OS::Thread thread_one( thread_one_task, 1, thread_one_stack, THREAD_STACK_SIZE );
static OS::Thread thread_two( thread_two_task, 2, thread_two_stack, THREAD_STACK_SIZE );


/**
  * \brief  Main application function
  * \retval int
  */
int main( void )
{   
   /* register the threads */
   OS::system_thread_manager.register_thread( &thread_one );
   OS::system_thread_manager.register_thread( &thread_two );
   
   /* configure the project specific HAL drivers and bootup the chip */
   PERIPHERAL_moduleInitialize( );

   /* enter the OS kernel */
   OS::enter_kernel( );

   /* main application loop */
   while ( 1 )
   {

   }

   return 0;
}


/**
 * \brief dummy thread one task
 * 
 */
static void thread_one_task( void )
{
   green_led.toggle( );
}


/**
 * \brief dummy thread two task
 * 
 */
static void thread_two_task( void )
{
   red_led.toggle( );
}
