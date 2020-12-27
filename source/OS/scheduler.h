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
    Scheduler(uint8_t max_thread_count);
    bool register_thread(Thread* thread);
    uint8_t get_thread_count(void);
    TaskControlBlock* get_active_tcb_ptr(void);

  private:
    uint8_t max_thread_count;
    uint8_t thread_count;
    std::unique_ptr<TaskControlBlock[]> task_control_blocks;
    TaskControlBlock* active_task;
};

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/

};  // namespace OS

#endif /* __SCHEDULER_H__ */
