/*! \file threads.c
*
*  \brief OS threading module.
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "threading.h"

namespace OS
{

/*********************************** Consts ********************************************/
#define CONTEXT_STACK_SIZE (16ul)  //!< number of default saved stack registers
#define PSR_THUMB_MODE     (0x01000000)  //!< set PSR register to THUMB

/************************************ Types ********************************************/
#pragma pack(1)
typedef struct {
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

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/
ThreadManager system_thread_manager;
TaskControlBlock* system_active_task = system_thread_manager.active_task;

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
Thread::Thread(THREAD_task_t task, uint32_t id, uint32_t* stack, uint32_t stack_size) {
    if ( (stack == nullptr) || (stack_size < CONTEXT_STACK_SIZE) ) {
        return;
    }

    this->status = ThreadStatus::active;
    this->stack = stack;
    this->stack_size = stack_size;
    this->id = id;
    this->task = task;

    /* initialize the threads stack with some setup values */
    ThreadContext_t* taskContext = reinterpret_cast<ThreadContext_t*>(&this->stack[this->stack_size - CONTEXT_STACK_SIZE]);

    /* set the task to run in thumb mode */
    taskContext->psr = PSR_THUMB_MODE;

    /* set the program counter to the function pointer for the thread */
    taskContext->pc = reinterpret_cast<uint32_t>(this->task);

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
 * \brief Construct a new Thread Manager:: Thread Manager object
 * 
 * \param max_threads max number of threads to allow
 */
ThreadManager::ThreadManager(void)
    : active_task(&task_control_blocks[0])
    , thread_count(0) { }

/**
 * \brief get the number of registered threads in the system
 * 
 * \retval uint8_t number of threads
 */
uint8_t ThreadManager::get_thread_count(void) {
    return thread_count;
}

/**
 * \brief get a copy of the active task control block
 * 
 * \retval TaskControlBlock
 */
TaskControlBlock* ThreadManager::get_active_task_ptr( void ) {            
    return active_task;
}

/**
 * \brief register a thread with a thread manager
 * 
 * \param thread the thread to register
 */
void ThreadManager::register_thread(Thread* thread) {
    if ( thread_count == 0 ) {
        /* add the the thread object*/
        task_control_blocks[thread_count].thread = thread;

        /* set the stack pointer to the pre-initialized values */
        task_control_blocks[thread_count].active_stack_pointer = thread->stack + thread->stack_size - CONTEXT_STACK_SIZE;

        /* set the next pointer */
        task_control_blocks[thread_count].next = nullptr;
        thread_count++;
    } else {
        if ( thread_count < system_max_threads ) {
            /* move the previous threads next pointer to the current thread */
            task_control_blocks[thread_count - 1].next = &task_control_blocks[thread_count];

            /* add the thread object and set the initial stack pointer */
            task_control_blocks[thread_count].thread = thread;
            task_control_blocks[thread_count].active_stack_pointer = thread->stack + thread->stack_size - CONTEXT_STACK_SIZE;

            /* set the current threads next pointer to the start of the list */
            task_control_blocks[thread_count].next = &task_control_blocks[0];

            thread_count++;
        }
    }
}

};  // namespace OS
