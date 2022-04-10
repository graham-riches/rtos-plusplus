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
#include "mutex.hpp"
#include "core_cm4.h"
#include <array>

/*********************************** Local Variables ********************************************/
constexpr std::size_t thread_stack_size = 512;
static os::binary_semaphore sem{0};
static std::array<uint32_t, thread_stack_size> thread_one_stack = {0};
static std::array<uint32_t, thread_stack_size> thread_two_stack = {0};

/*********************************** Function Definitions ********************************************/
/**
 * \brief First task to blink two LEDs
 */
static void thread_one_task() {    
    while ( true ) {        
        green_led.toggle();
        blue_led.toggle();                        
        os::this_thread::sleep_for_msec(1000);        
        sem.release();
    }
}

/**
 * \brief Second task to blink the other two LEDs
 */
static void thread_two_task() {    
    while ( true ) {
        sem.acquire();        
        red_led.toggle();
        orange_led.toggle();        
    }
}

/**
  * \brief  Main application function  
  */
int main() {        
    // Create two threads
    os::thread thread_one(thread_one_task, 1, thread_one_stack.data(), thread_stack_size);
    os::thread thread_two(thread_two_task, 2, thread_two_stack.data(), thread_stack_size);

    // Configure peripherals
    initialize_peripherals();

    // Jump into the RTOS kernel
    os::kernel::setup();
    os::kernel::enter();

    //!< NOTE: should never reach here!
    return 0;
}
