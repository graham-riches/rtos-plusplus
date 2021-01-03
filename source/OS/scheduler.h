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
#include "threading.h"
#include "system_clock.h"
#include <memory>


namespace OS
{
/************************************ Types ********************************************/
/**
 * \brief function pointer for setting a pending interrupt with the scheduler. This injects
 *        the HW dependency into the scheduler at run-time so that it can be tested more easily.
 */
typedef void (*SetPendingFP)(void);

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
  public:
    Scheduler(SystemClock& clock_source, uint8_t max_thread_count, SetPendingFP set_pending);
    void run(void);
    void sleep_thread(uint32_t ticks);
    bool register_thread(Thread* thread);
    uint8_t get_thread_count(void);
    TaskControlBlock* get_active_tcb_ptr(void);
    
  private:
    SystemClock &clock;
    uint32_t last_ticks;
    uint8_t max_thread_count;
    uint8_t thread_count;
    std::unique_ptr<TaskControlBlock[]> task_control_blocks;
    TaskControlBlock* active_task;
    SetPendingFP set_pending;
};


};  // namespace OS

#endif /* __SCHEDULER_H__ */
