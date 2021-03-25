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
constexpr uint8_t thread_stack_size = 128;

/*********************************** Local Functions ********************************************/
static void thread_one_task(void *arguments);
static void thread_two_task(void *arguments);
static void thread_three_task(void *arguments);

/*********************************** Local Variables ********************************************/
/* create the thread stacks */
static uint32_t thread_one_stack[thread_stack_size] = {0};
static uint32_t thread_two_stack[thread_stack_size] = {0};
static uint32_t thread_three_stack[thread_stack_size] = {0};

/* create the threads */
static OS::Thread thread_one(thread_one_task, nullptr, 1, thread_one_stack, thread_stack_size);
static OS::Thread thread_two(thread_two_task, nullptr, 2, thread_two_stack, thread_stack_size);
static OS::Thread thread_three(thread_three_task, nullptr, 3, thread_three_stack, thread_stack_size);

/**
  * \brief  Main application function
  * \retval int
  */
int main(void) {
    /* register the threads */
    OS::scheduler.register_thread(&thread_one);
    OS::scheduler.register_thread(&thread_two);
    OS::scheduler.register_thread(&thread_three);

    /* configure the project specific HAL drivers and bootup the chip, clocks etc. */
    initialize_peripherals();

    /* enter the OS kernel */
    OS::setup_kernel();
    OS::enter_kernel();

    /* NOTE: should never reach here! */
    return 0;
}

/**
 * \brief first task to blink two LEDs
 */
static void thread_one_task(void *arguments) {
    PARAMETER_NOT_USED(arguments);
    while ( true ) {        
        debug_port.send("a\n");
        green_led.toggle();
        blue_led.toggle();
        OS::sleep_thread(10);
    }
}

/**
 * \brief second task to blink the other two LEDs
 */
static void thread_two_task(void *arguments) {
    PARAMETER_NOT_USED(arguments);
    while ( true ) {
        debug_port.send("b\n");
        red_led.toggle();
        orange_led.toggle();
        OS::sleep_thread(10);
    }
}

/**
 * \brief third task to just hang out an do nothing in case the other threads are sleeping
 * \TODO this should be internal to the OS!! ***
 * 
 * \param arguments pointer to task arguments
 */
static void thread_three_task(void *arguments) {
    PARAMETER_NOT_USED(arguments);
    while (true) {
    }
}


