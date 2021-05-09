/**
 * \file scheduler.cpp
 * \author Graham Riches (graham.riches@live.com)
 * \brief contains the singleton implementation of the os scheduler
 * \version 0.1
 * \date 2021-05-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/********************************** Includes *******************************************/
#include "scheduler.h"
#include "cm4_port.h"
#include "system_clock.h"
#include "thread_impl.h"

namespace os
{

/********************************** Constants *******************************************/
constexpr uint8_t internal_thread_stack_size = 128;

/********************************** Function Declarations *******************************************/
static void internal_thread_task(void* arguments);

/********************************** Local Variables *******************************************/
static uint32_t internal_thread_stack[internal_thread_stack_size] = {0};
static os::thread internal_thread(internal_thread_task, nullptr, 0xFFFF, internal_thread_stack, internal_thread_stack_size);

/********************************** Function Definitions *******************************************/
//!> default construct the scheduler
scheduler::scheduler()
    : scheduler_impl(system_clock::get(), MAX_THREAD_COUNT, set_pending_context_switch, is_context_switch_pending) {
    set_internal_task(&internal_thread);
}

//!< get a reference to the scheduler
scheduler& scheduler::get() {
    static scheduler os_scheduler;
    return os_scheduler;
}

//!< run the scheduler algorithm
void scheduler::update() {
    auto& self = get();
    self.run();
}

//!< register a new thread
void scheduler::register_new_thread(thread* thread) {
    auto& self = get();
    self.register_thread(thread);
}

//!< sleep the calling thread
void scheduler::sleep(uint32_t ticks) {
    auto& self = get();
    DISABLE_INTERRUPTS();
    self.sleep_thread(ticks);
    ENABLE_INTERRUPTS();
}

//!< get the active task control block
scheduler::TaskControlBlock* scheduler::get_active_task_control_block() {
    auto& self = get();
    return self.get_active_tcb_ptr();
}

//< get the pending task control block
scheduler::TaskControlBlock* scheduler::get_pending_task_control_block() {
    auto& self = get();
    return self.get_pending_tcb_ptr();
}



/**
 * \brief empty internal thread task to run when all other threads are sleeping
 * 
 * \param arguments 
 */
static void internal_thread_task(void* arguments) {
    PARAMETER_NOT_USED(arguments);
    while ( true ) {
    }
}

};  // namespace os
