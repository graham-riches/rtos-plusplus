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
 * \param clock_source system clock source for running the scheduler
 * \param max_thread_count max number of threads to allow
 * \param set_pending function pointer to the function to set a pending context switch interrupt
 * \param check_pending function pointer to check if an interrupt is already pending
 */
Scheduler::Scheduler(SystemClock& clock_source, uint8_t max_thread_count, SetPendingInterrupt set_pending, IsInterruptPending check_pending)
    : clock(clock_source)    
    , max_thread_count(max_thread_count)
    , set_pending(set_pending)
    , check_pending(check_pending)
    , last_tick(0)
    , thread_count(0)
    , task_control_blocks(std::make_unique<TaskControlBlock[]>(max_thread_count))
    , active_task(&task_control_blocks[0])
    , pending_task(nullptr) { }


/**
 * \brief run the scheduling algorithm and signal any context switches to the PendSV handler if required.
 */
void Scheduler::run(void){
    uint32_t current_tick{clock.get_ticks()};    
    uint32_t ticks{current_tick - last_tick}; 

    for (uint8_t thread = 0; thread < thread_count; thread++){
        auto tcb = &task_control_blocks[thread];

        //!< pick up any threads that are waking up from sleep
        if (tcb->thread->get_status() == ThreadStatus::suspended) {
            tcb->suspended_ticks_remaining -= ticks;
            if ((tcb->suspended_ticks_remaining) <= 0){
                tcb->thread->set_status(ThreadStatus::pending); 
            }
        }
    }
    
    //!< pick up any pending and context switch if required
    if ( !check_pending()) {
        for (uint8_t thread = 0; thread < thread_count; thread++){
            auto tcb = &task_control_blocks[thread];        
            if (tcb->thread->get_status() == ThreadStatus::pending){       
                active_task->thread->set_status(ThreadStatus::pending);     
                context_switch_to(tcb);
                break;
            }
        }
    }

    last_tick = current_tick;
}

/**
 * \brief trigger a context switch to the thread pointer to by the task control block
 * 
 * \param tcb pointer to the task control block
 */
void Scheduler::context_switch_to(TaskControlBlock* tcb) {
    pending_task = tcb;
    tcb->thread->set_status(ThreadStatus::active);
    active_task = pending_task;
    set_pending();
}

/**
 * \brief sleep the active thread for a set number of ticks. This will trigger a context switch to the
 *        next active thread as the current thread will be put to sleep!
 * 
 * \param ticks how many ticks to sleep the active thread for
 */
void Scheduler::sleep_thread(uint32_t ticks){    
    active_task->suspended_ticks_remaining = ticks;
    active_task->thread->set_status(OS::ThreadStatus::suspended);

    /* find the next active thread if a context switch is not already pending */
    if (!check_pending()){
        for (uint8_t thread = 0; thread < thread_count; thread++){
            auto tcb = &task_control_blocks[thread];
            if ((tcb->thread->get_status() == ThreadStatus::pending) && (tcb != active_task)) {
                context_switch_to(tcb);
                break;
            }
        }
    }
}

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
 * \brief Get the pending task pointer
 * 
 * \retval TaskControlBlock* 
 */
TaskControlBlock* Scheduler::get_pending_tcb_ptr(void) {
    return pending_task;
}

/**
 * \brief register a thread with a thread manager
 * 
 * \param thread the thread to register
 * \retval returns true if the thread registration was successful
 */
bool Scheduler::register_thread(Thread* thread) {
    bool retval{false};
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
