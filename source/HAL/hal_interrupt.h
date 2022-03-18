/*! \file hal_interrupt.h
*
*  \brief hal_interrupt C++ API to interface with all system interrupts.
*  \details registers a specific function callback with context (aimed for C++)
*           in a interrupts vector stored in SRAM. Each low-level interrupt function is
*           manually defined, which maintains the ability of the NVIC to handle priority
*           but each function just calls a registered ISR out of the same array.
*
*
*  \author Graham Riches
*/

#pragma once

/********************************** Includes *******************************************/
#include "common.h"
#include "stm32f4xx.h"

namespace HAL
{
/*********************************** Consts ********************************************/
constexpr uint32_t nvic_priority_group_level = 0;  //!< Enable 16 configurable priority levels and no subpriorities
constexpr uint32_t active_vector_mask = 0xFF;      //!< Mask for currently active interrupt vector
constexpr uint8_t external_interrupt_offset = 16;  //!< Offset for mappping interrupt numbers to enums defined below

/************************************ Types ********************************************/

/**
 * \brief enumeration of preemption priority groupings.
 * \note an interrupt in the highest priority grouping (level_1) cannot be interrupt by a lower priory grouping interrpt
 */
enum class isr_preemption_priority : uint8_t {
    level_1 = 0,
    level_2,
    level_3,
    level_4,
    level_5,
    level_6,
    level_7,
    level_8,
    level_9,
    level_10,
    level_11,
    level_12,
    level_13,
    level_14,
    level_15,
    level_16,
};

/**
 * \brief Helper class to manage NVIC
 */
template <typename isr_enumeration>
class interrupt_manager {
  public:
    /**
     * \brief Construct a new Interrupt Manager:: Interrupt Manager object
     * \note this will configure the microcontroller NVIC priority grouping
     */
    interrupt_manager() {
        NVIC_SetPriorityGrouping(nvic_priority_group_level);
    }

    /**
     * \brief set the priority grouping and subpriority for an interrupt
     * 
     * \param interrupt the interrupt to set the priority for
     * \param preemption_priority premption priority level
     */
    inline void set_priority(isr_enumeration interrupt, isr_preemption_priority preemption_priority) {
        uint32_t priority = NVIC_EncodePriority(nvic_priority_group_level, static_cast<uint32_t>(preemption_priority), 1UL);
        IRQn external_interrupt_number = static_cast<IRQn>(static_cast<uint8_t>(interrupt) - external_interrupt_offset);
        NVIC_SetPriority(external_interrupt_number, priority);
        NVIC_EnableIRQ(external_interrupt_number);
    }
};
};  // namespace HAL
