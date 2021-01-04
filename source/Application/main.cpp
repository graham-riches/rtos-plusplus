/*! \file main.c
*
*  \brief Main application code
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "board.h"
#include "common.h"
#include "debug_port.h"
#include "peripherals.h"
#include "stm32f4xx.h"
#include "kernel.h"
#include <stdio.h>
#include <string.h>


/*********************************** Consts ********************************************/
#define THREAD_STACK_SIZE 128

/************************************ Types ********************************************/

/*********************************** Macros ********************************************/

/*********************************** Local Functions ********************************************/
static void thread_one_task(void *arguments);
static void thread_two_task(void *arguments);

/*********************************** Local Variables ********************************************/
/* create the thread stacks */
static uint32_t thread_one_stack[THREAD_STACK_SIZE] = {0};
static uint32_t thread_two_stack[THREAD_STACK_SIZE] = {0};

/* create the threads */
static OS::Thread thread_one(thread_one_task, nullptr, 1, thread_one_stack, THREAD_STACK_SIZE);
static OS::Thread thread_two(thread_two_task, nullptr, 2, thread_two_stack, THREAD_STACK_SIZE);

/**
  * \brief  Main application function
  * \retval int
  */
int main(void) {
    /* register the threads */
    OS::scheduler.register_thread(&thread_one);
    OS::scheduler.register_thread(&thread_two);

    /* configure the project specific HAL drivers and bootup the chip, clocks etc. */
    initialize_peripherals();

    /* enter the OS kernel */
    OS::enter_kernel();

    /* NOTE: should never reach here! */
    return 0;
}

/**
 * \brief dummy thread one task
 * 
 */
static void thread_one_task(void *arguments) {
    PARAMETER_NOT_USED(arguments);
    while ( true ) {
        green_led.toggle();
        blue_led.toggle();
        OS::sleep_thread(250);
    }
}

/**
 * \brief dummy thread two task
 * 
 */
static void thread_two_task(void *arguments) {
    PARAMETER_NOT_USED(arguments);
    while ( true ) {
        red_led.toggle();
        orange_led.toggle();
        OS::sleep_thread(250);
    }
}
