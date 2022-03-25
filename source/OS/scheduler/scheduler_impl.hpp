/*! \file scheduler_impl.hpp
*
*  \brief simple cooperative scheduler for multi-tasking implementation
*
*
*  \author Graham Riches
*/

#pragma once

/********************************** Includes *******************************************/
#include "common.hpp"
#include "system_clock.hpp"
#include "thread_impl.hpp"

#include <memory>
#include <optional>

namespace os
{
//!< Scheduler implementation details
class scheduler_impl {
  public:
    /**
     * \brief task control block for thread management 
     */
    struct TaskControlBlock {
        uint32_t* active_stack_pointer;
        TaskControlBlock* next;
        thread* thread_ptr;
        int32_t suspended_ticks_remaining;
    };

    /**
    * \brief function pointer for setting a pending interrupt with the scheduler. This injects
    *        the HW dependency into the scheduler at run-time so that it can be tested more easily.
    */
    typedef void (*SetPendingInterrupt)(void);

    /**
    * \brief function pointer to call a function that checks if an interrupt for a context switch is already pending
    *        so that another request is not able to clobber it
    */
    typedef bool (*IsInterruptPending)(void);

    /**
     * \brief Construct a new scheduler
     * 
     * \param clock_source system clock source for running the scheduler
     * \param max_thread_count max number of threads to allow
     * \param set_pending function pointer to the function to set a pending context switch interrupt
     * \param check_pending function pointer to check if an interrupt is already pending
     */
    scheduler_impl(system_clock_impl& clock_source, uint8_t max_thread_count, SetPendingInterrupt set_pending, IsInterruptPending check_pending)
        : m_clock(clock_source)
        , max_thread_count(max_thread_count)
        , set_pending(set_pending)
        , check_pending(check_pending)
        , last_tick(0)
        , thread_count(0)
        , task_control_blocks(std::make_unique<TaskControlBlock[]>(max_thread_count))
        , active_task(&task_control_blocks[0])
        , pending_task(nullptr)
        , internal_task() { }

    /**
     * \brief run the scheduling algorithm and signal any context switches to the PendSV handler if required.
     */
    void run(void) {
        uint32_t current_tick{m_clock.get_ticks()};
        uint32_t ticks{current_tick - last_tick};

        for ( uint8_t thread = 0; thread < thread_count; thread++ ) {
            auto tcb = &task_control_blocks[thread];

            //!< pick up any threads that are waking up from sleep
            if ( tcb->thread_ptr->get_status() == thread::status::sleeping ) {
                tcb->suspended_ticks_remaining -= ticks;
                if ( (tcb->suspended_ticks_remaining) <= 0 ) {
                    tcb->thread_ptr->set_status(thread::status::pending);
                }
            }
        }

        //!< pick up any pending tasks and context switch if required
        if ( !check_pending() ) {
            for ( uint8_t thread = 0; thread < thread_count; thread++ ) {
                auto tcb = &task_control_blocks[thread];
                if ( tcb->thread_ptr->get_status() == thread::status::pending ) {
                    active_task->thread_ptr->set_status(thread::status::pending);
                    context_switch_to(tcb);
                    break;
                }
            }
        }

        last_tick = current_tick;
    }

    /**
     * \brief sleep the active thread for a set number of ticks. This will trigger a context switch to the
     *        next active thread as the current thread will be put to sleep!
     * 
     * \param ticks how many ticks to sleep the active thread for
     */
    void sleep_thread(uint32_t ticks) {
        active_task->suspended_ticks_remaining = ticks;
        active_task->thread_ptr->set_status(os::thread::status::sleeping);
        jump_to_next_pending_task();
    }

    /**
     * \brief suspends the calling thread and triggers a context switch to the next available thread
     */
    void suspend_thread() {
        active_task->thread_ptr->set_status(os::thread::status::suspended);
        jump_to_next_pending_task();
    }

    /**
     * \brief register a thread with the scheduler
     * 
     * \param thread the thread to register
     * \retval returns true if the thread registration was successful
     */
    bool register_thread(thread* thread) {
        bool retval{false};
        if ( thread_count < max_thread_count ) {
            /* add the the thread object and it's stack pointer to the next empty task control block */
            task_control_blocks[thread_count].thread_ptr = thread;
            task_control_blocks[thread_count].active_stack_pointer = thread->get_stack_ptr();

            /* setup the next pointers */
            task_control_blocks[thread_count].next = (thread_count == 0) ? nullptr : &task_control_blocks[0];

            /* move the last task blocks next pointer to the current block */
            if ( thread_count > 0 ) {
                task_control_blocks[thread_count - 1].next = &task_control_blocks[thread_count];
            }

            thread_count++;
            retval = true;
        }
        return retval;
    }

    /**
     * \brief register a thread as the internal OS thread that will run when all other threads are sleeping
     * \param thread pointer to the thread to register
     * \todo could maybe move this into the task constructor?
     */
    void set_internal_task(thread* thread) {
        internal_task.thread_ptr = thread;
        internal_task.active_stack_pointer = thread->get_stack_ptr();
        internal_task.suspended_ticks_remaining = 0;
    }

    /**
     * \brief get the number of registered threads in the system
     * 
     * \retval uint8_t number of threads
     */
    uint8_t get_registered_thread_count(void) {
        return thread_count;
    }

    /**
     * \brief Get the max thread count that the scheduler can support
     * 
     * \retval uint8_t mas supported threads
     */
    uint8_t get_max_thread_count(void) {
        return max_thread_count;
    }

    /**
     * \brief get the active task control block
     * 
     * \retval TaskControlBlock
     */
    TaskControlBlock* get_active_tcb_ptr(void) {
        return active_task;
    }

    /**
     * \brief Get the pending task pointer
     * 
     * \retval TaskControlBlock* 
     */
    TaskControlBlock* get_pending_tcb_ptr(void) {
        return pending_task;
    }

    /**
     * \brief get a task control block by thread id
     * 
     * \param id the thread id
     * \retval TaskControlBlock* pointer to the task control block     
     */
    std::optional<TaskControlBlock*> get_task_by_id(uint32_t id) {
        for ( uint8_t thread = 0; thread < thread_count; thread++ ) {
            auto tcb = &task_control_blocks[thread];
            auto thread_ptr = tcb->thread_ptr;
            if ( thread_ptr->get_id() == id ) {
                return tcb;
            }
        }
        return {};
    }

  private:
    /**
     * \brief trigger a context switch to the thread pointer to by the task control block
     * 
     * \param tcb pointer to the task control block
     */
    void context_switch_to(TaskControlBlock* tcb) {
        pending_task = tcb;
        tcb->thread_ptr->set_status(thread::status::active);
        active_task = pending_task;
        set_pending();
    }

    /**
     * \brief jump to the next available task
     */
    void jump_to_next_pending_task() {
        /* find the next active thread if a context switch is not already pending */
        if ( !check_pending() ) {
            for ( uint8_t thread = 0; thread < thread_count; thread++ ) {
                auto tcb = &task_control_blocks[thread];
                if ( (tcb->thread_ptr->get_status() == thread::status::pending) && (tcb != active_task) ) {
                    context_switch_to(tcb);
                    return;
                }
            }
        }

        /* all threads are sleeping so context switch to the internal OS thread */
        context_switch_to(&internal_task);
    }

    system_clock_impl& m_clock;
    uint8_t max_thread_count;
    SetPendingInterrupt set_pending;
    IsInterruptPending check_pending;
    uint32_t last_tick;
    uint8_t thread_count;
    std::unique_ptr<TaskControlBlock[]> task_control_blocks;
    TaskControlBlock* active_task;
    TaskControlBlock* pending_task;
    TaskControlBlock internal_task;
};

};  // namespace os
