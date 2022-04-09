/**
 * \file interrupt_lock_guard.hpp
 * \author Graham Riches
 * \brief RAII Container for managing interrupt enable/disable
 * \version 0.1
 * \date 2022-04-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

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