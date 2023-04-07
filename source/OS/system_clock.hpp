// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Graham Riches

#pragma once

#include <cstdint>

namespace os
{
/**
 * \brief System clock class for managing task timing
 */
class system_clock {
  public:
    /**
     * \brief default constructor for the system clock
     */
    system_clock()
        : m_elapsed_ticks(0) { }

    /**
     * \brief get the elapsed system tick time
     * 
     * \retval uint32_t elapsed ticks
     */
    uint32_t get_ticks() {
        return m_elapsed_ticks;
    }

    /**
     * \brief update the system clock
     * 
     * \param ticks number of elapsed ticks since last update
     */
    void update(uint32_t ticks) {
        m_elapsed_ticks = m_elapsed_ticks + ticks;
    }

  private:
    volatile uint32_t m_elapsed_ticks;
};

};  // namespace os
