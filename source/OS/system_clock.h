/*! \file system_clock.h
*
*  \brief manages the OS time monitoring and clocking
*
*
*  \author Graham Riches
*/

#ifndef __SYSTEM_CLOCK_H__
#define __SYSTEM_CLOCK_H__

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

    private:
        volatile uint32_t elapsed_ticks;        
};



};  // namespace OS


#endif /* __SYSTEM_CLOCK_H__ */
