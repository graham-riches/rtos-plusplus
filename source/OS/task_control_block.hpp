// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Graham Riches

#pragma once

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
};  // namespace os