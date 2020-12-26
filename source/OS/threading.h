/*! \file threading.h
*
*  \brief creation of OS threading primatives
*
*
*  \author Graham Riches
*/

#ifndef __THREADS_H__
#define __THREADS_H__

/********************************** Includes *******************************************/
#include "common.h"

namespace OS
{

/*********************************** Consts ********************************************/
constexpr uint8_t system_max_threads = 2;  //!< max number of threads in the OS

/************************************ Types ********************************************/
/**
 * \brief function pointer for thread task
 * 
 */
typedef void (*THREAD_task_t)(void);

/**
 * \brief enumeration of possible thread statuses
 */
enum class ThreadStatus : unsigned {
    active = 0,
    suspended,
};

/**
 * \brief class for system threads
 */
class Thread {
  private:
    uint32_t id;
    THREAD_task_t task;
    ThreadStatus status;

  public:
    uint32_t* stack;
    uint32_t stack_size;

    Thread(THREAD_task_t task, uint32_t id, uint32_t* stack, uint32_t stack_size);
    void set_status(ThreadStatus status);
    ThreadStatus get_status(void);
};

/**
 * \brief task control block for thread management 
 */
#pragma pack(0)
struct TaskControlBlock {
    uint32_t* active_stack_pointer;
    TaskControlBlock* next;
    Thread* thread;
};
#pragma pack(1)

/**
 * \brief class for managing application threads
 */
class ThreadManager {
  public:    
    ThreadManager();
    void register_thread(Thread* thread);
    uint8_t get_thread_count(void);
    TaskControlBlock *get_active_task_ptr( void );


  private:
    TaskControlBlock* active_task;
    uint8_t thread_count;
    TaskControlBlock task_control_blocks[system_max_threads];
};

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/
extern ThreadManager system_thread_manager;   //!< NOTE: other code components should interact with the thread manager

/****************************** Functions Prototype ************************************/

};  // namespace OS

#endif /* __THREADS_H__ */
