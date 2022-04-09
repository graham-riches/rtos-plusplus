/*! \file thread.cpp
*
*  \brief OS threading module.
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "thread.hpp"
#include "scheduler.hpp"

namespace os
{
/*********************************** Consts ********************************************/
constexpr uint32_t CONTEXT_STACK_SIZE  = 16ul;      //!< Number of default saved stack registers
constexpr uint32_t PSR_THUMB_MODE      = 0x01000000; //!< set PSR register to THUMB

/****************************** Method Definitions ***********************************/

thread::thread(task_pointer task_ptr, uint32_t id, uint32_t* stack_ptr, uint32_t stack_size) 
: task_ptr(task_ptr)
, id(id)
, stack_top_ptr(stack_ptr)
, stack_ptr(stack_ptr)
, stack_size(stack_size)
, task_status(status::pending) {
    // TODO: static assert pointers?    

    // Initialize the threads stack with some setup values
    register_context* task_context = reinterpret_cast<register_context*>(&stack_ptr[stack_size - CONTEXT_STACK_SIZE]);

    // Set the task to run in thumb mode
    task_context->psr = PSR_THUMB_MODE;

    // set the program counter to the function pointer for the thread    
    task_context->pc = static_cast<uint32_t>(reinterpret_cast<std::uintptr_t>(task_ptr));

    // Initialize the thread stack to junk for debugging
    task_context->r0 = 0;
    task_context->r1 = 1;
    task_context->r2 = 2;
    task_context->r3 = 3;
    task_context->r4 = 4;
    task_context->r5 = 5;
    task_context->r6 = 6;
    task_context->r7 = 7;
    task_context->r8 = 8;
    task_context->r9 = 9;
    task_context->r10 = 10;
    task_context->r11 = 11;
    task_context->r12 = 12;

    // Set the initial value of the active stack pointer to the fake context added above.
    // When the thread is unsuspended, the processor will restore the saved context     
    this->stack_ptr = this->stack_top_ptr + stack_size - CONTEXT_STACK_SIZE;
    
    os::scheduler::register_new_thread(this);
}

//!< Update the threads state
void thread::set_status(thread::status status) {
    this->task_status = status;
}

// Get the threads state
thread::status thread::get_status() const {
    return task_status;
}

/// Get the thread ID
uint32_t thread::get_id() const {
    return id;
}

// Get the stack pointer
uint32_t* thread::get_stack_ptr(){
    return stack_ptr;
}
};  // namespace os
