/*! \file threading.h
*
*  \brief creation of OS threading primatives
*
*
*  \author Graham Riches
*/

#pragma once

/********************************** Includes *******************************************/
#include "common.h"

namespace OS
{
/************************************ Types ********************************************/
/**
 * \brief class for system threads
 */
class Thread {
  public:
    /**
     * \brief enumeration of possible thread states
     */
    enum class Status : unsigned {
        active = 0,
        suspended,
        pending,
    };    

    /**
     * \brief function pointer to the thread task
     */
    using TaskPointer = void (*)(void *arguments);

    /**
     * \brief Construct a new Thread object
     * \todo I would like to make this more generic so that any invokable can be passed in like a lambda, etc.
     * 
     * \param task_ptr function pointer to the main task function
     * \param arguments arguments to bind to the task function
     * \param id thread id
     * \param stack_ptr pointer to the thread stack
     * \param stack_size size of the threads stack
     */
    Thread(TaskPointer task_ptr, void *arguments, uint32_t id, uint32_t* stack_ptr, uint32_t stack_size);

    /**
     * \brief Set the thread's status
     * 
     * \param status new status to set
     */
    void set_status(Status status);

    /**
     * \brief Get the thread's status
     * 
     * \retval Status of the thread
     */
    Status get_status(void);

    /**
     * \brief Get the stack pointer for the thread
     * 
     * \retval uint32_t* pointer to the thread's stack
     */
    uint32_t* get_stack_ptr(void);

    /**
     * \brief Get the thread id
     * 
     * \retval uint32_t thread id
     */
    uint32_t get_id(void);
  
  private:
    const TaskPointer task_ptr;
    void* task_arguments_ptr;
    const uint32_t id;
    uint32_t* const stack_top_ptr;
    uint32_t* stack_ptr;
    const uint32_t stack_size;
    Status status;
};

};  // namespace OS
