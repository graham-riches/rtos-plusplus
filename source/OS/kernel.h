/*! \file kernel.h
*
*  \brief this file provides a single include point for the user application to access
*         the OS. All OS variables should be created in this module to control the 
*         initialization order.
*  \note  components that need to be accessed by ISRs or C routines need to be declared
*         with extern C linkage here
*
*  \author Graham Riches
*/

#pragma once

/********************************** Includes *******************************************/
#include "common.h"
#include "scheduler.h"
#include "semaphore.h"
#include "system_clock.h"
#include "threading.h"

/****************************** Constants ************************************/
#define SYSTEM_MAX_THREADS (8ul)  //!< maximum system thread count

namespace os
{
/* C linkage global variables for handling context switching in interrupts */
extern "C"
{
    extern Scheduler::TaskControlBlock* system_active_task;
    extern Scheduler::TaskControlBlock* system_pending_task;
}

/****************************** Functions Declaration and Factory Methods ************************************/
namespace kernel
{

/**
 * \brief setup the kernel     
 */
void setup();

/**
 * \brief start the kernel     
 */
void enter();

/**
 * \brief Set the systick frequency for the OS scheduling
 * 
 * \param ticks number of ticks between events
 */
void set_systick_frequency(uint32_t ticks);

};  // namespace kernel


namespace system_clock
{

/**
 * \brief update the system clock time
 * 
 * \param ticks number of elapsed ticks since last clock update
 */
void update(uint32_t ticks);

};  // namespace system_clock


namespace this_thread
{

/**
 * \brief sleep the calling thread for a number of ticks
 * 
 * \param ticks number of ticks to sleep for
 */
void sleep_for(uint32_t ticks);

};  // namespace this_thread


namespace scheduler
{

/**
 * \brief register a new thread with the operating system
 * 
 * \param thread pointer to the thread
 */
void register_thread(Thread* thread);

/**
 * \brief run the scheduler
 */
void run();

};  // namespace scheduler
};  // namespace os
