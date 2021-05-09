/*! \file main.cpp
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
#include "os.h"
#include <stdio.h>
#include <string.h>


/*********************************** Consts ********************************************/
constexpr uint8_t thread_stack_size = 128;

/*********************************** Local Function Declarations ********************************************/
static void thread_one_task(void *arguments);
static void thread_two_task(void *arguments);

/*********************************** Local Variables ********************************************/
/* create the thread stacks */
static uint32_t thread_one_stack[thread_stack_size] = {0};
static uint32_t thread_two_stack[thread_stack_size] = {0};

/* create the threads */
static os::thread thread_one(thread_one_task, nullptr, 1, thread_one_stack, thread_stack_size);
static os::thread thread_two(thread_two_task, nullptr, 2, thread_two_stack, thread_stack_size);

/*********************************** Function Definitions ********************************************/
/**
  * \brief  Main application function
  * \retval int
  */
int main(void) {
    //!< register the threads
    os::scheduler::register_thread(&thread_one);
    os::scheduler::register_thread(&thread_two);

    //!< configure the project specific HAL drivers and bootup the chip, clocks etc.
    initialize_peripherals();    

    //!< jump into the RTOS kernel
    os::kernel::setup();
    os::kernel::enter();

    //!< NOTE: should never reach here
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
        os::this_thread::sleep_for(100);
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
        os::this_thread::sleep_for(100);
    }
}
