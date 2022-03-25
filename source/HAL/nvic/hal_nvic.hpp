/*! \file hal_nvic.hpp
*   \brief Manages Nested Vectored Interrupt Controller
*
*  \author Graham Riches
*/

#pragma once

/********************************** Includes *******************************************/
#include "stm32f4xx.h"
#include <cstdint>

namespace HAL::nvic
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

/************************************ Function Declarations ********************************************/
/**
 * \brief Initializes the NVIC to a enable 16 priority levels by default. Note: this can be changed, however it requires
 *        careful implementation of priority levels!
 */
static inline void initialize() {
    NVIC_SetPriorityGrouping(nvic_priority_group_level);
}

/**
 * \brief set the priority grouping and subpriority for an interrupt
 * 
 * \param interrupt the interrupt to set the priority for
 * \param preemption_priority premption priority level
 */
template <typename isr_enumeration>
static inline void set_priority(isr_enumeration interrupt, isr_preemption_priority preemption_priority) {
    uint32_t priority = NVIC_EncodePriority(nvic_priority_group_level, static_cast<uint32_t>(preemption_priority), 1UL);
    IRQn external_interrupt_number = static_cast<IRQn>(static_cast<uint8_t>(interrupt) - external_interrupt_offset);
    NVIC_SetPriority(external_interrupt_number, priority);
    NVIC_EnableIRQ(external_interrupt_number);
}
};  // namespace HAL
