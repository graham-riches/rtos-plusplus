// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Graham Riches

#pragma once

/********************************** Includes *******************************************/
#include "scheduler.hpp"
#include "task_control_block.hpp"

namespace os
{

extern "C" {
/********************************** Global Objects and Variables *******************************************/
extern task_control_block* system_active_task;
extern task_control_block* system_pending_task;
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