/*! \file threading.h
*
*  \brief creation of OS threading primatives
*
*
*  \author Graham Riches
*/

#ifndef __THREADS_H__
#define __THREADS_H__

/********************************** Includes *******************************************/
#include "common.h"

namespace OS
{
/************************************ Types ********************************************/
#pragma pack(1)
/**
 * \brief contains the packed ordering of the processor register state during context switches.
 *        This is super helpful for debugging as you can pre-fill a thread's context with a custom
 *        default register state.
 */
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


/**
 * \brief function pointer for a thread task
 */
typedef void (*task_ptr_t)(void *arguments);

/**
 * \brief enumeration of possible thread statuses
 */
enum class ThreadStatus : unsigned {
    active = 0,
    suspended,
};

/**
 * \brief class for system threads
 */
class Thread {
  public:
    Thread(task_ptr_t task_ptr, void *arguments, uint32_t id, uint32_t* stack_ptr, uint32_t stack_size);
    void set_status(ThreadStatus status);
    ThreadStatus get_status(void);
    uint32_t* get_stack_ptr(void);
    uint32_t get_id(void);
  
  private:
    task_ptr_t task_ptr;
    void* task_arguments_ptr;
    uint32_t id;
    uint32_t* const stack_top_ptr;
    uint32_t* stack_ptr;
    uint32_t stack_size;
    ThreadStatus status;
};


};  // namespace OS

#endif /* __THREADS_H__ */
