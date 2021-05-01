/*! \file system_clock.h
*
*  \brief manages the OS time monitoring and clocking
*
*
*  \author Graham Riches
*/

#pragma once

/********************************** Includes *******************************************/
#include "common.h"

namespace OS
{
/********************************** Types *******************************************/
/**
 * \brief system clock class for managing task timing
 */
class SystemClock{
    public:
        SystemClock(void);
        uint32_t get_ticks(void);
        void update(uint32_t ticks);
        void start(void);

    private:
        volatile uint32_t elapsed_ticks;
        bool running;  
};



};  // namespace OS
