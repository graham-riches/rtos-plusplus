/*! \file scheduler.h
*
*  \brief simple cooperative scheduler for multi-tasking
*
*
*  \author Graham Riches
*/

#pragma once

/********************************** Includes *******************************************/
#include "common.h"
#include "system_clock.h"
#include "threading.h"

#include <memory>
#include <optional>

namespace OS
{
/**
 * \brief class for managing application threads and scheduling context switches
 * \note  there is nothing preventing the user from creating multiple instances of this class, which has pros and cons.
 *        On one hand, it's a lot easier to test, on the other there should only be a single instance of this class
 *        in the application so instead, a single extern instance exists in the kernel, which supplies all the outwards
 *        facing components of the OS to the user application.
 */
class Scheduler {
  public:
    /**
     * \brief task control block for thread management 
     */
    struct TaskControlBlock {
        uint32_t* active_stack_pointer;
        TaskControlBlock* next;
        Thread* thread;
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
     * \brief Construct a new Scheduler::Scheduler object
     * 
     * \param clock_source system clock source for running the scheduler
     * \param max_thread_count max number of threads to allow
     * \param set_pending function pointer to the function to set a pending context switch interrupt
     * \param check_pending function pointer to check if an interrupt is already pending
     */
    Scheduler(SystemClock& clock_source, uint8_t max_thread_count, SetPendingInterrupt set_pending, IsInterruptPending check_pending);

    /**
     * \brief run the scheduling algorithm and signal any context switches to the PendSV handler if required.
     */
    void run(void);
    
    /**
     * \brief sleep the active thread for a set number of ticks. This will trigger a context switch to the
     *        next active thread as the current thread will be put to sleep!
     * 
     * \param ticks how many ticks to sleep the active thread for
     */
    void sleep_thread(uint32_t ticks);

    /**
     * \brief register a thread with the scheduler
     * 
     * \param thread the thread to register
     * \retval returns true if the thread registration was successful
     */
    bool register_thread(Thread* thread);

    /**
     * \brief register a thread as the internal OS thread that will run when all other threads are sleeping
     * \param thread pointer to the thread to register
     * \todo could maybe move this into the task constructor?
     */
    void set_internal_task(Thread* thread);

    /**
     * \brief get the number of registered threads in the system
     * 
     * \retval uint8_t number of threads
     */
    uint8_t get_thread_count(void);

    /**
     * \brief get the active task control block
     * 
     * \retval TaskControlBlock
     */
    TaskControlBlock* get_active_tcb_ptr(void);

    /**
     * \brief Get the pending task pointer
     * 
     * \retval TaskControlBlock* 
     */
    TaskControlBlock* get_pending_tcb_ptr(void);

    /**
     * \brief get a task control block by thread id
     * 
     * \param id the thread id
     * \retval TaskControlBlock* pointer to the task control block     
     */
    std::optional<TaskControlBlock*> get_task_by_id(uint32_t id);

  private:
    /**
     * \brief trigger a context switch to the thread pointer to by the task control block
     * 
     * \param tcb pointer to the task control block
     */
    void context_switch_to(TaskControlBlock* tcb);

    SystemClock& clock;
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

};  // namespace OS
