/*! \file scheduler.c
*
*  \brief A simple cooperative scheduler for concurrent tasks.
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "scheduler.h"
#include "threading.h"

namespace OS
{
/****************************** Functions Definition ***********************************/

/**
 * \brief Construct a new Scheduler::Scheduler object
 * 
 * \param max_thread_count max number of threads to allow
 */
Scheduler::Scheduler(uint8_t max_thread_count)
    : max_thread_count(max_thread_count)
    , thread_count(0)
    , task_control_blocks(std::make_unique<TaskControlBlock[]>(max_thread_count))
    , active_task(&task_control_blocks[0]) { }

/**
 * \brief get the number of registered threads in the system
 * 
 * \retval uint8_t number of threads
 */
uint8_t Scheduler::get_thread_count(void) {
    return thread_count;
}

/**
 * \brief get the active task control block
 * 
 * \retval TaskControlBlock
 */
TaskControlBlock* Scheduler::get_active_tcb_ptr(void) {
    return active_task;
}

/**
 * \brief register a thread with a thread manager
 * 
 * \param thread the thread to register
 * \retval returns true if the thread registration was successful
 */
bool Scheduler::register_thread(Thread* thread) {
    bool retval = false;
    if ( thread_count < max_thread_count ) {
        /* add the the thread object and it's stack pointer to the next empty task control block */
        task_control_blocks[thread_count].thread = thread;
        task_control_blocks[thread_count].active_stack_pointer = thread->get_stack_ptr();

        /* setup the next pointers */
        task_control_blocks[thread_count].next = (thread_count == 0) ? nullptr : &task_control_blocks[0];

        /* move the last task blocks next pointer to the current block */
        if ( thread_count > 0 ) {
            task_control_blocks[thread_count - 1].next = &task_control_blocks[thread_count];
        }

        thread_count++;
        retval = true;
    }
    return retval;
}

};  // namespace OS
