/*! \file threads.h
*
*  \brief threads module functions and variables declarations.
*
*
*  \author Graham Riches
*/

#ifndef __THREADS_H__
#define __THREADS_H__

/********************************** Includes *******************************************/
#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

namespace OS
{

/*********************************** Consts ********************************************/
constexpr uint8_t max_system_threads = 2; //!< max number of threads to support


/************************************ Types ********************************************/
/**
 * \brief function pointer for thread task
 * 
 */
typedef void (*THREAD_task_t)( void );


/**
 * \brief class for system threads
 */
class Thread
{
   private:
      uint32_t id;
      THREAD_task_t task;
   
   public:
      uint32_t *stack;
      uint32_t stack_size;

      Thread( THREAD_task_t task, uint32_t id, uint32_t *stack, uint32_t stack_size );      
      
};

/**
 * \brief task control block for thread management 
 */
struct TaskControlBlock
{
   TaskControlBlock *next;
   Thread *thread;
   uint32_t *active_stack_pointer;
};


/**
 * \brief class for managing application threads
 */
class ThreadManager
{
   private:      
      uint8_t thread_count;
      TaskControlBlock task_control_blocks[max_system_threads];

   public:      
      TaskControlBlock *activeTask;

      ThreadManager( );
      void register_thread( Thread *thread );
};

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/
extern ThreadManager system_thread_manager;

/****************************** Functions Prototype ************************************/


};  // namespace OS


#ifdef __cplusplus
}
#endif

#endif /* __THREADS_H__ */
