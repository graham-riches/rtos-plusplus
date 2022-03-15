/**
 * \file cm4_port.h
 * \author Graham Riches (graham.riches@live.com)
 * \brief definitions for cortex-m4 functions required for OS port
 * \version 0.1
 * \date 2021-05-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

/********************************** Includes *******************************************/
#include <cstdint>

namespace os
{

#define DISABLE_INTERRUPTS() __asm("CPSID I\n");
#define ENABLE_INTERRUPTS()  __asm("CPSIE I\n");

/********************************** Function Declarations *******************************************/
/**
 * \brief Set a pending context switch interrupt (platform dependent)
 * 
 */
void set_pending_context_switch(void);

/**
 * \brief check if a context switch is pending (platform dependent)
 * 
 * \retval true/false
 */
bool is_context_switch_pending(void);

};  // namespace os
