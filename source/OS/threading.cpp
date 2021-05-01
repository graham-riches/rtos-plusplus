/*! \file threads.cpp
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

/****************************** Method Definitions ***********************************/

Thread::Thread(TaskPointer task_ptr, void *arguments, uint32_t id, uint32_t* stack_ptr, uint32_t stack_size) 
: task_ptr(task_ptr)
, task_arguments_ptr(arguments)
, id(id)
, stack_top_ptr(stack_ptr)
, stack_ptr(stack_ptr)
, stack_size(stack_size)
, status(Status::pending) {
    
    assert(task_ptr != nullptr);
    assert(stack_ptr != nullptr);
    assert(stack_size > 0);

    /* initialize the threads stack with some setup values */
    RegisterContext* taskContext = reinterpret_cast<RegisterContext*>(&stack_ptr[stack_size - CONTEXT_STACK_SIZE]);

    /* set the task to run in thumb mode */
    taskContext->psr = PSR_THUMB_MODE;

    /* set the program counter to the function pointer for the thread */
    taskContext->pc = *reinterpret_cast<uint32_t*>(task_ptr);

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


void Thread::set_status(Status status) {
    this->status = status;
}


Thread::Status Thread::get_status(void) {
    return status;
}


uint32_t Thread::get_id(void) {
    return id;
}


uint32_t* Thread::get_stack_ptr(void){
    return stack_ptr;
}



};  // namespace OS
