/**
 * \file system_clock.h
 * \author Graham Riches (graham.riches@live.com)
 * \brief singleton accessor for the system clock
 * \version 0.1
 * \date 2021-05-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once


/********************************** Includes *******************************************/
#include "system_clock_impl.h"

namespace os
{
/**
 * \brief singleton instance of the system clock
 * 
 */
class system_clock : private system_clock_impl {
  public:
    /**
     * \brief static singleton accessor for the system clock
     * 
     * \retval system_clock& reference to the system clock object
     */
    static system_clock& get();

    /**
     * \brief get the elapsed system tick time
     * 
     * \retval uint32_t elapsed ticks
     */
    static uint32_t get_elapsed_ticks(void);

    /**
     * \brief update the system clock
     * 
     * \param ticks number of elapsed ticks since last update
     */
    static void update_sytem_ticks(uint32_t ticks);

    /**
     * \brief initialize the system clock
     */
    static void initialize();

  private:
    /**
     * \brief Construct a new system clock object
     * 
     */
    system_clock();
};

}  // namespace os
