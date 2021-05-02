/*! \file system_clock.cpp
*
*  \brief operating system clocking and timing functionality.
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "system_clock.h"

namespace os
{
/****************************** Function Definitions ***********************************/
/**
 * \brief default constructor for the system clock
 */
SystemClock::SystemClock(void)
: elapsed_ticks(0) {}

/**
 * \brief get the elapsed system tick time
 * 
 * \retval uint32_t elapsed ticks
 */
uint32_t SystemClock::get_ticks(void){
    return elapsed_ticks;
}

/**
 * \brief update the system clock
 * 
 * \param ticks number of elapsed ticks since last update
 */
void SystemClock::update(uint32_t ticks){
    if ( running ){
        elapsed_ticks += ticks;
    }    
}

/**
 * \brief start the system clock
 */
void SystemClock::start(void) {
    running = true;
}

};  // namespace os
