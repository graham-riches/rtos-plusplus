// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Graham Riches

#pragma once

/********************************** Includes *******************************************/
#include "device_port.hpp"

namespace os
{

/**
 * \brief RAII Lock guard style container for managing enabling and disabling interrupts
 */
struct interrupt_guard {
    /**
     * \brief Construct a new interrupt guard object, which disables interrupts     
     */
    interrupt_guard() {
        DISABLE_INTERRUPTS();
    }

    /**
     * \brief Destroy the interrupt guard object, which re-enables interrupts     
     */
    ~interrupt_guard() {
        ENABLE_INTERRUPTS();
    }
};

};  // namespace os