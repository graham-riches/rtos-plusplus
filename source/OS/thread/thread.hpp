/*! \file thread.hpp
*
*  \brief creation of OS threading primatives
*
*
*  \author Graham Riches
*/

#pragma once

/********************************** Includes *******************************************/
#include <cstdint>

namespace os
{
/************************************ Types ********************************************/
/**
 * \brief class for system threads
 */
class thread {
  public:
    /**
     * \brief contains the packed ordering of the processor register state during context switches.
     *        This is super helpful for debugging as you can pre-fill a thread's context with a custom
     *        default register state.
     */
    struct register_context {
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
    };

    /**
     * \brief enumeration of possible thread states
     */
    enum class status : unsigned {
        active = 0,
        suspended,
        sleeping,
        pending,
    };    

    /**
     * \brief function pointer to the thread task
     */
    using task_pointer = void (*)();

    /**
     * \brief Construct a new Thread object
     * \todo I would like to make this more generic so that any invokable can be passed in like a lambda, etc.
     * 
     * \param task_ptr function pointer to the main task function     
     * \param id thread id
     * \param stack_ptr pointer to the thread stack
     * \param stack_size size of the threads stack
     */
    thread(task_pointer task_ptr, uint32_t id, uint32_t* stack_ptr, uint32_t stack_size);

    /**
     * \brief Set the thread's status
     * 
     * \param task_status new status to set
     */
    void set_status(status task_status);

    /**
     * \brief Get the thread's status
     * 
     * \retval Status of the thread
     */
    status get_status() const;

    /**
     * \brief Get the stack pointer for the thread
     * 
     * \retval uint32_t* pointer to the thread's stack
     */
    uint32_t* get_stack_ptr();

    /**
     * \brief Get the thread id
     * 
     * \retval uint32_t thread id
     */
    uint32_t get_id() const;
  
  private:
    const task_pointer task_ptr;    
    const uint32_t id;
    uint32_t* const stack_top_ptr;
    uint32_t* stack_ptr;
    const uint32_t stack_size;
    status task_status;
};

};  // namespace os
