/*! \file system_clock_impl.h
*
*  \brief manages the OS time monitoring and clocking
*
*
*  \author Graham Riches
*/

#pragma once

/********************************** Includes *******************************************/
#include "common.h"

namespace os
{
/********************************** Types *******************************************/
/**
 * \brief system clock class for managing task timing
 */
class system_clock_impl {
  public:
    /**
     * \brief default constructor for the system clock
     */
    system_clock_impl(void)
        : elapsed_ticks(0)
        , running(false) { }

    /**
     * \brief get the elapsed system tick time
     * 
     * \retval uint32_t elapsed ticks
     */
    uint32_t get_ticks(void) {
        return elapsed_ticks;
    }

    /**
     * \brief update the system clock
     * 
     * \param ticks number of elapsed ticks since last update
     */
    void update(uint32_t ticks) {
        if ( running ) {
            elapsed_ticks += ticks;
        }
    }

    /**
     * \brief start the system clock
     */
    void start(void) {
        running = true;
    }

  private:
    volatile uint32_t elapsed_ticks;
    bool running;
};

};  // namespace os
