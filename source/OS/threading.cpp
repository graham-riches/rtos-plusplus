/*! \file threads.c
*
*  \brief OS threading module.
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "threading.h"
#include <cassert>

namespace OS
{

/*********************************** Consts ********************************************/
#define CONTEXT_STACK_SIZE (16ul)        //!< number of default saved stack registers
#define PSR_THUMB_MODE     (0x01000000)  //!< set PSR register to THUMB
#define SYSTEM_MAX_THREADS (16ul)        //!< number of allowed threads

/************************************ Types ********************************************/


/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/


/******************************** Local Variables **************************************/

/****************************** Functions Prototype ************************************/

/****************************** Functions Definition ***********************************/

/**
 * \brief Construct a new Thread:: Thread object
 * 
 * \param task_ptr function pointer to the task function
 * \param arguments arguments for the task
 * \param id thread id
 * \param stack_ptr pointer to the top of the stack for the thread
 * \param stack_size size of the thread's stack
 */
Thread::Thread(task_ptr_t task_ptr, void *arguments, uint32_t id, uint32_t* stack_ptr, uint32_t stack_size) 
: task_ptr(task_ptr)
, task_arguments_ptr(arguments)
, id(id)
, stack_top_ptr(stack_ptr)
, stack_ptr(stack_ptr)
, stack_size(stack_size)
, status(ThreadStatus::active) {
    
    assert(("thread task pointer is null", task_ptr != nullptr));
    assert(("thread stack pointer is null", stack_ptr != nullptr));
    assert(("stack size is zero", stack_size > 0));

    /* initialize the threads stack with some setup values */
    ThreadContext_t* taskContext = reinterpret_cast<ThreadContext_t*>(&stack_ptr[stack_size - CONTEXT_STACK_SIZE]);

    /* set the task to run in thumb mode */
    taskContext->psr = PSR_THUMB_MODE;

    /* set the program counter to the function pointer for the thread */
    taskContext->pc = reinterpret_cast<uint32_t>(task_ptr);

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

    /* set the initial value of the active stack pointer to the fake context added above.
       When the thread is unsuspended, the processor will restore the saved context 
    */
    this->stack_ptr = this->stack_top_ptr + stack_size - CONTEXT_STACK_SIZE;
}

/**
 * \brief update the thread status
 * 
 * \param status value
 */
void Thread::set_status(ThreadStatus status) {
    this->status = status;
}

/**
 * \brief get the current thread status
 * 
 * \retval ThreadStatus 
 */
ThreadStatus Thread::get_status(void) {
    return status;
}

/**
 * \brief get a thread's id
 * 
 * \retval uint32_t the thread id
 */
uint32_t Thread::get_id(void) {
    return id;
}

/**
 * \brief get the current stack pointer for the thread
 * 
 * \retval uint32_t* pointer to the active stack pointer address
 */
uint32_t* Thread::get_stack_ptr(void){
    return stack_ptr;
}



};  // namespace OS
