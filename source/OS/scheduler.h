/*! \file scheduler.h
*
*  \brief simple cooperative scheduler for multi-tasking
*
*
*  \author Graham Riches
*/

#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

/********************************** Includes *******************************************/
#include "common.h"
#include "system_clock.h"
#include "threading.h"
#include <memory>

namespace OS
{
/************************************ Types ********************************************/
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
 * \brief class for managing application threads and scheduling context switches
 * \note  there is nothing preventing the user from creating multiple instances of this class, which has pros and cons.
 *        On one hand, it's a lot easier to test, on the other there should only be a single instance of this class
 *        in the application so instead, a single extern instance exists in the kernal, which supplies all the outwards
 *        facing components of the OS to the user application.
 */
class Scheduler {
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

  public:
    Scheduler(SystemClock& clock_source, uint8_t max_thread_count, SetPendingInterrupt set_pending, IsInterruptPending check_pending);
    void run(void);
    void context_switch_to(TaskControlBlock* tcb);
    void sleep_thread(uint32_t ticks);
    bool register_thread(Thread* thread);
    void set_internal_task(Thread* thread);
    uint8_t get_thread_count(void);
    TaskControlBlock* get_active_tcb_ptr(void);
    TaskControlBlock* get_pending_tcb_ptr(void);
    TaskControlBlock* get_task_by_id(uint32_t id);

  private:
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

#endif /* __SCHEDULER_H__ */
