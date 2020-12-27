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

namespace OS
{
/*********************************** Consts ********************************************/

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
 * \brief class for managing application threads
 */
class Scheduler {
  public:        
    Scheduler(uint8_t max_thread_count);
    bool register_thread(Thread* thread);
    uint8_t get_thread_count(void);
    TaskControlBlock* get_active_tcb_ptr( void );

  private:
    uint8_t max_thread_count;
    uint8_t thread_count;
    std::unique_ptr<TaskControlBlock[]> task_control_blocks;
    TaskControlBlock* active_task;        
};


/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/
extern Scheduler scheduler;   //!< NOTE: other code components should interact with the thread manager
extern "C"
{
    extern TaskControlBlock* system_active_task;  //!< global task pointer to the current task control block
}


/****************************** Functions Prototype ************************************/
void enter_kernel(void);
void set_systick_frequency(uint32_t ticks);

};  // namespace OS

#endif /* __SCHEDULER_H__ */
