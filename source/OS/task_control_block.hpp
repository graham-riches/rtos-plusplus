/**
 * \file task_control_block.hpp
 * \author Graham Riches
 * \brief Contains the definition of a task control block for the scheduler
 * \version 0.1
 * \date 2022-04-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

/********************************** Includes *******************************************/
#include "common.hpp"
#include "thread.hpp"

namespace os
{

/**
 * \brief task control block for thread management 
 */
struct task_control_block {
    uint32_t* active_stack_pointer;
    task_control_block* next;
    thread* thread_ptr;
    int32_t suspended_ticks_remaining;
};
};