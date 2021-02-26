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

#ifndef __KERNEL_H__
#define __KERNEL_H__

/********************************** Includes *******************************************/
#include "common.h"
#include "scheduler.h"
#include "threading.h"
#include "system_clock.h"


/****************************** Constants ************************************/    
#define SYSTEM_MAX_THREADS (8ul) //!< maximum system thread count

namespace OS
{
/****************************** Global Variables ************************************/
extern Scheduler scheduler;
extern SystemClock core_clock;

/* C linkage global variables */
extern "C" {
    extern TaskControlBlock* system_active_task;
    extern TaskControlBlock* system_pending_task;
}


/****************************** Functions Declarations ************************************/
void setup_kernel(void);
void enter_kernel(void);
void set_systick_frequency(uint32_t ticks);
void sleep_thread(uint32_t ticks);

};  // namespace OS


#endif /* __KERNEL_H__ */
