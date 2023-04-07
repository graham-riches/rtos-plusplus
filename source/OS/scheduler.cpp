// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Graham Riches

/********************************** Includes *******************************************/
#include "scheduler.hpp"
#include "device_port.hpp"
#include "thread.hpp"

namespace os
{

/********************************** Constants *******************************************/
constexpr uint8_t internal_thread_stack_size = 128;

/********************************** Function Declarations *******************************************/
static void internal_thread_task();

/********************************** Local Variables *******************************************/
static uint32_t internal_thread_stack[internal_thread_stack_size] = {0};
static os::thread internal_thread(internal_thread_task, 0xFFFF, internal_thread_stack, internal_thread_stack_size);

/********************************** Function Definitions *******************************************/
//!> default construct the scheduler
scheduler::scheduler()
    : scheduler_impl(MAX_THREAD_COUNT, set_pending_context_switch, is_context_switch_pending)
    , m_locked(false) {
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
    if ( !self.m_locked ) {        
        self.run();
    }
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
task_control_block* scheduler::get_active_task_control_block() {
    auto& self = get();
    return self.get_active_tcb_ptr();
}

//!< get the pending task control block
task_control_block* scheduler::get_pending_task_control_block() {
    auto& self = get();
    return self.get_pending_tcb_ptr();
}

//!< Suspend the calling thread indefinitely
void scheduler::suspend_calling_thread() {
    auto& self = get();
    self.suspend_thread();
}

//!< Get elapsed system tick count
uint32_t scheduler::get_elapsed_ticks() {
    auto& self = get();
    return self.m_clock.get_ticks();
}

/**
 * \brief update the system clock
 * 
 * \param ticks number of elapsed ticks since last update
 */
void scheduler::update_system_ticks(uint32_t ticks) {
    auto &self = get();
    self.m_clock.update(ticks);
}

//!< lock the scheduler
void scheduler::lock() {    
    DISABLE_INTERRUPTS();
    auto& self = get();
    self.m_locked = true;
    ENABLE_INTERRUPTS();
}

//!< unlock the scheduler
void scheduler::unlock() {
    DISABLE_INTERRUPTS();
    auto& self = get();
    self.m_locked = false;
    ENABLE_INTERRUPTS();
}

/**
 * \brief empty internal thread task to run when all other threads are sleeping  
 */
static void internal_thread_task() {    
    while ( true ) {
    }
}

};  // namespace os
