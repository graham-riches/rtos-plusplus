/*! \file threads.c
*
*  \brief OS threading module.
*
*
*  \author Graham Riches
*/


/********************************** Includes *******************************************/
#include "threading.h"
#include "board.h"

namespace OS
{

/*********************************** Consts ********************************************/
#define CONTEXT_STACK_SIZE (16ul)       //!< number of default saved stack registers
#define PSR_THUMB_MODE     (0x01000000) //!< set PSR register to THUMB

/************************************ Types ********************************************/
#pragma pack(1)
typedef struct
{
   uint32_t r4;
   uint32_t r5;
   uint32_t r6;
   uint32_t r7;
   uint32_t r8;
   uint32_t r9;
   uint32_t r10;   
   uint32_t r11;
   uint32_t r0;
   uint32_t r1;
   uint32_t r2;
   uint32_t r3;
   uint32_t r12;
   uint32_t lr;
   uint32_t pc;
   uint32_t psr;
} ThreadContext_t;
#pragma pack(0)


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
static ThreadManager system_thread_manager;
TaskControlBlock *system_active_task = system_thread_manager.activeTask;

/******************************** Local Variables **************************************/


/****************************** Functions Prototype ************************************/


/****************************** Functions Definition ***********************************/
/**
 * \brief Construct a new Thread:: Thread object
 * 
 * \param task pointer to the thread task
 * \param id id of the thread
 * \param stack pointer to the task stack
 * \param stack_size size of the task stack
 */
Thread::Thread( THREAD_task_t task, uint32_t id, uint32_t *stack, uint32_t stack_size )
{
   if ( (stack == nullptr) || (stack_size < CONTEXT_STACK_SIZE) )
   {
      //!< TODO: need to create an error here
      return;
   }

   this->status = ThreadStatus::active;
   this->stack = stack;
   this->stack_size = stack_size;
   this->id = id;
   this->task = task;

   /* initialize the threads stack with some setup values */
   ThreadContext_t *taskContext = reinterpret_cast<ThreadContext_t *>( &this->stack[this->stack_size - CONTEXT_STACK_SIZE ] );

   /* set the task to run in thumb mode */
   taskContext->psr = PSR_THUMB_MODE;

   /* set the program counter to the function pointer for the thread */
   taskContext->pc = reinterpret_cast<uint32_t>( this->task );

   /* set some garbo values to watch the registers during debugging */
   taskContext->r0 = 0;
   taskContext->r1 = 1;
   taskContext->r2 = 2;
   taskContext->r3 = 3;
   taskContext->r4 = 4;
   taskContext->r5 = 5;
   taskContext->r6 = 6;
   taskContext->r7 = 7;
   taskContext->r8 = 8;
   taskContext->r9 = 9;
   taskContext->r10 = 10;
   taskContext->r11 = 11;
   taskContext->r12 = 12;
}


/**
 * \brief Construct a new Thread Manager:: Thread Manager object
 * 
 * \param max_threads max number of threads to allow
 */
ThreadManager::ThreadManager( void )
{
   this->thread_count = 0;
   this->activeTask = &this->task_control_blocks[0];
}


/**
 * \brief register a thread with a thread manager
 * 
 * \param thread the thread to register
 */
void ThreadManager::register_thread( Thread *thread )
{
   if ( this->thread_count == 0 )
   {
      /* add the the thread object*/
      this->task_control_blocks[this->thread_count].thread = thread;
      
      /* set the stack pointer to the pre-initialized values */
      this->task_control_blocks[this->thread_count].active_stack_pointer = thread->stack + thread->stack_size - CONTEXT_STACK_SIZE ;

      /* set the next pointer */
      this->task_control_blocks[this->thread_count].next = nullptr;
      this->thread_count++;
   }
   else
   {
      /* move the previous threads next pointer to the current thread */
      this->task_control_blocks[this->thread_count - 1].next = &this->task_control_blocks[this->thread_count];

      /* add the thread object and set the initial stack pointer */
      this->task_control_blocks[this->thread_count].thread = thread;
      this->task_control_blocks[this->thread_count].active_stack_pointer = thread->stack + thread->stack_size - CONTEXT_STACK_SIZE;

      /* set the current threads next pointer to the start of the list */
      this->task_control_blocks[this->thread_count].next = &this->task_control_blocks[0];

      this->thread_count++;
   }
}

/**
 * \brief register a new thread into the OS
 * 
 * \param thread pointer to the thread class
 */
void register_thread( Thread *thread )
{
   system_thread_manager.register_thread( thread );
}

};
