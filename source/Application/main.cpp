/*! \file main.cpp
*
*  \brief Main application code
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "board.hpp"
#include "common.hpp"
#include "debug_port.hpp"
#include "peripherals.hpp"
#include "stm32f4xx.h"
#include "os.hpp"
#include "semaphore.hpp"

/*********************************** Consts ********************************************/
constexpr uint8_t thread_stack_size = 128;

/*********************************** Local Function Declarations ********************************************/
static void thread_one_task();
static void thread_two_task();

/*********************************** Local Variables ********************************************/
static uint32_t thread_one_stack[thread_stack_size] = {0};
static uint32_t thread_two_stack[thread_stack_size] = {0};
static os::counting_semaphore<1> sem(0);

/*********************************** Function Definitions ********************************************/
/**
  * \brief  Main application function  
  */
int main(void) {
    // Create two threads
    os::thread thread_one(thread_one_task, 1, thread_one_stack, thread_stack_size);
    os::thread thread_two(thread_two_task, 2, thread_two_stack, thread_stack_size);

    // Configure peripherals
    initialize_peripherals();

    // Jump into the RTOS kernel
    os::kernel::setup();
    os::kernel::enter();

    //!< NOTE: should never reach here!
    return 0;
}

/**
 * \brief First task to blink two LEDs
 */
static void thread_one_task() {    
    while ( true ) {        
        green_led.toggle();
        blue_led.toggle();        
        os::scheduler::sleep(1000);
        sem.release();
    }
}

/**
 * \brief Second task to blink the other two LEDs
 */
static void thread_two_task() {    
    while ( true ) {
        sem.acquire();
        debug_port.log_message("In second thread\r\n");
        red_led.toggle();
        orange_led.toggle();        
    }
}
