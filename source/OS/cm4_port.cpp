/**
 * \file cm4_port.cpp
 * \author Graham Riches (graham.riches@live.com)
 * \brief port os scheduling and context switching functionality for cortex M4
 * \version 0.1
 * \date 2021-05-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/********************************** Includes *******************************************/
#include "hal_interrupt.h"
#include "stm32f4xx.h"
#include "os.h"

namespace os
{

/********************************** Function Definitions *******************************************/
/**
 * \brief Set the PendSV interrupt flag in the NVIC to trigger a context switch
 */
void set_pending_context_switch(void) {
    system_pending_task = scheduler::get_pending_task_control_block();
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

/**
 * \brief check if an interrupt is already pending for a context switch
 * 
 * \retval true/false 
 */
bool is_context_switch_pending(void) {
    return static_cast<bool>(SCB->ICSR & SCB_ICSR_PENDSVSET_Msk);
}

};  // namespace os