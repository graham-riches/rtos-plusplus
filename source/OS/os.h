/**
 * \file os.h
 * \author Graham Riches (graham.riches@live.com)
 * \brief operating system definitions and API
 * \version 0.1
 * \date 2021-05-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

/********************************** Includes *******************************************/
#include "scheduler.h"
#include "system_clock.h"

namespace os
{

extern "C" {
/********************************** Global Objects and Variables *******************************************/
extern scheduler::TaskControlBlock* system_active_task;
extern scheduler::TaskControlBlock* system_pending_task;
}

/********************************** Function Definitions *******************************************/
namespace kernel
{
/**
 * \brief setup the kernel 
 * 
 */
void setup();

/**
 * \brief launch the rtos kernel
 * 
 */
void enter();

/**
 * \brief Set the systick timer frequency
 * 
 * \param ticks of the main sysclock per interrupt
 */
void set_systick_frequency(uint32_t ticks);

};  // namespace kernel
};  // namespace os