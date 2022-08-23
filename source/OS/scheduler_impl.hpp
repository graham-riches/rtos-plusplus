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
#include "task_control_block.hpp"
#include "thread.hpp"
#include "system_clock_impl.hpp"
#include <memory>
#include <optional>


namespace os
{
//!< Scheduler implementation details
class scheduler_impl {
  public:
    /**
    * \brief Function pointer for setting a pending interrupt with the scheduler. This injects
    *        the HW dependency into the scheduler at run-time so that it can be tested more easily.
    */
    using set_pending_interrupt = void (*)();

    /**
    * \brief Function pointer to call a function that checks if an interrupt for a context switch is already pending
    *        so that another request is not able to clobber it
    */
    using is_interrupt_pending = bool (*)();

    /**
     * \brief Construct a new scheduler
     * 
     * \param clock_source System clock source for running the scheduler
     * \param max_thread_count Max number of threads to allow
     * \param set_pending Function pointer to the function to set a pending context switch interrupt
     * \param check_pending Function pointer to check if an interrupt is already pending
     */
    scheduler_impl(unsigned max_thread_count,
                   set_pending_interrupt set_pending,
                   is_interrupt_pending check_pending)        
        : m_max_thread_count(max_thread_count)
        , m_set_pending(set_pending)
        , m_check_pending(check_pending)
        , m_last_tick(0)
        , m_thread_count(0)
        , m_task_control_blocks(std::make_unique<task_control_block[]>(m_max_thread_count))
        , m_active_task(&m_task_control_blocks[0])
        , m_pending_task(nullptr)
        , m_internal_task() { }

    /**
     * \brief Run the scheduling algorithm and signal any context switches to the PendSV handler if required.
     */
    void run() {
        uint32_t current_tick{m_clock.get_ticks()};
        uint32_t ticks{current_tick - m_last_tick};

        for ( unsigned thread = 0; thread < m_thread_count; thread++ ) {
            auto tcb = &m_task_control_blocks[thread];

            // Pick up any threads that are waking up from sleep
            if ( tcb->thread_ptr->get_status() == thread::status::sleeping ) {
                tcb->suspended_ticks_remaining -= ticks;
                if ( (tcb->suspended_ticks_remaining) <= 0 ) {
                    tcb->thread_ptr->set_status(thread::status::pending);
                }
            }
        }

        // Pick up any pending tasks and context switch if required
        if ( !m_check_pending() ) {
            for ( unsigned thread = 0; thread < m_thread_count; thread++ ) {
                auto tcb = &m_task_control_blocks[thread];
                if ( tcb->thread_ptr->get_status() == thread::status::pending ) {
                    m_active_task->thread_ptr->set_status(thread::status::pending);
                    context_switch_to(tcb);
                    break;
                }
            }
        }

        m_last_tick = current_tick;
    }

    /**
     * \brief Sleep the active thread for a set number of ticks. This will trigger a context switch to the
     *        next active thread as the current thread will be put to sleep!
     * 
     * \param ticks How many ticks to sleep the active thread for
     */
    void sleep_thread(uint32_t ticks) {
        m_active_task->suspended_ticks_remaining = ticks;
        m_active_task->thread_ptr->set_status(os::thread::status::sleeping);
        jump_to_next_pending_task();
    }

    /**
     * \brief Suspends the calling thread and triggers a context switch to the next available thread
     */
    void suspend_thread() {
        m_active_task->thread_ptr->set_status(os::thread::status::suspended);
        jump_to_next_pending_task();
    }

    /**
     * \brief Register a thread with the scheduler
     * 
     * \param thread The thread to register
     * \retval Returns true if the thread registration was successful
     */
    bool register_thread(thread* thread) {
        bool retval{false};
        if ( m_thread_count < m_max_thread_count ) {
            // Add the the thread object and its stack pointer to the next empty task control block
            m_task_control_blocks[m_thread_count].thread_ptr = thread;
            m_task_control_blocks[m_thread_count].active_stack_pointer = thread->get_stack_ptr();

            // Setup the next pointers
            m_task_control_blocks[m_thread_count].next = (m_thread_count == 0) ? nullptr : &m_task_control_blocks[0];

            // Move the last task blocks next pointer to the current block
            if ( m_thread_count > 0 ) {
                m_task_control_blocks[m_thread_count - 1].next = &m_task_control_blocks[m_thread_count];
            }

            // If there is no pending thread, assign this thread as pending
            // if (m_pending_task == nullptr) {
            //     m_pending_task = &m_task_control_blocks[m_thread_count];
            // }

            m_thread_count++;
            retval = true;
        }
        return retval;
    }

    /**
     * \brief Register a thread as the internal OS thread that will run when all other threads are sleeping
     * \param thread Pointer to the thread to register
     * \todo could maybe move this into the task constructor?
     */
    void set_internal_task(thread* thread) {
        m_internal_task.thread_ptr = thread;
        m_internal_task.active_stack_pointer = thread->get_stack_ptr();
        m_internal_task.suspended_ticks_remaining = 0;
    }

    /**
     * \brief Get the number of registered threads in the system
     * 
     * \retval unsigned Number of registered threads
     */
    unsigned get_registered_thread_count() const {
        return m_thread_count;
    }

    /**
     * \brief Get the max thread count that the scheduler can support
     * 
     * \retval unsigned Max supported threads
     */
    unsigned get_max_thread_count() const {
        return m_max_thread_count;
    }

    /**
     * \brief Get the active task control block
     * 
     * \retval task_control_block Active task control block pointer
     */
    task_control_block* get_active_tcb_ptr() const {
        return m_active_task;
    }

    /**
     * \brief Get the pending task pointer
     * 
     * \retval task_control_block* Pending task control block pointer
     */
    task_control_block* get_pending_tcb_ptr() const {
        return m_pending_task;
    }

    /**
     * \brief Get a task control block by thread id
     * 
     * \param id The thread id
     * \retval optional<task_control_block*> Maybe the thread pointer if the id exists
     */
    std::optional<task_control_block*> get_task_by_id(uint32_t id) {
        for ( unsigned thread = 0; thread < m_thread_count; thread++ ) {
            auto tcb = &m_task_control_blocks[thread];
            auto thread_ptr = tcb->thread_ptr;
            if ( thread_ptr->get_id() == id ) {
                return tcb;
            }
        }
        return {};
    }

    /**
     * \brief get the elapsed system tick time
     * 
     * \retval uint32_t elapsed ticks
     */
    uint32_t get_elapsed_ticks() {
        return m_clock.get_ticks();
    }

    /**
     * \brief update the system clock
     * 
     * \param ticks number of elapsed ticks since last update
     */
    void update_system_ticks(uint32_t ticks) {
        m_clock.update(ticks);
    }

  protected:
    /**
     * \brief trigger a context switch to the thread pointer to by the task control block
     * 
     * \param tcb pointer to the task control block
     */
    void context_switch_to(task_control_block* tcb) {
        m_pending_task = tcb;
        tcb->thread_ptr->set_status(thread::status::active);
        m_active_task = m_pending_task;
        m_set_pending();
    }

    /**
     * \brief jump to the next available task
     */
    void jump_to_next_pending_task() {
        // Find the next active thread if a context switch is not already pending
        if ( !m_check_pending() ) {
            for ( unsigned thread = 0; thread < m_thread_count; thread++ ) {
                auto tcb = &m_task_control_blocks[thread];
                if ( (tcb->thread_ptr->get_status() == thread::status::pending) && (tcb != m_active_task) ) {
                    context_switch_to(tcb);
                    return;
                }
            }
        }

        // All threads are sleeping so context switch to the internal OS thread
        context_switch_to(&m_internal_task);
    }

    system_clock_impl m_clock;
    unsigned m_max_thread_count;
    set_pending_interrupt m_set_pending;
    is_interrupt_pending m_check_pending;
    uint32_t m_last_tick;
    unsigned m_thread_count;
    std::unique_ptr<task_control_block[]> m_task_control_blocks;
    task_control_block* m_active_task;
    task_control_block* m_pending_task;
    task_control_block m_internal_task;
};

};  // namespace os
