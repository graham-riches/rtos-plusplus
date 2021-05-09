/**
 * \file system_clock.cpp
 * \author Graham Riches (graham.riches@live.com)
 * \brief contains singleton instance of the system clock
 * \version 0.1
 * \date 2021-05-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/********************************** Includes *******************************************/
#include "system_clock.h"

namespace os
{
/********************************** Function Definitions *******************************************/
//!< create the system clock singleton
system_clock::system_clock()
    : system_clock_impl() { }

//!< get a reference to the system clock singleton
system_clock& system_clock::get() {
    static system_clock clock;
    return clock;
}

//!< get elapsed clock ticks
uint32_t system_clock::get_elapsed_ticks(void) {
    auto& self = get();
    return self.get_ticks();
}

//!< update the clock with elapsed ticks
void system_clock::update_sytem_ticks(uint32_t ticks) {
    auto& self = get();
    self.update(ticks);
}

//!< start the system clock
void system_clock::initialize() {
    auto& self = get();
    self.start();
}

};  // namespace os