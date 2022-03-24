/*! \file hal_exti.h
*
*  \brief external interrupts management for GPIO lines
*
*
*  \author Graham Riches
*/

#pragma once

/********************************** Includes *******************************************/
#include "hal_gpio.h"
#include "stm32f4xx.h"

namespace HAL::exti
{
/************************************ Types ********************************************/
/**
 * \brief Enumeration of EXTI lines
 */
enum class line : unsigned {
    line_0 = 0,
    line_1,
    line_2,
    line_3,
    line_4,
    line_5,
    line_6,
    line_7,
    line_8,
    line_9,
    line_10,
    line_11,
    line_12,
    line_13,
    line_14,
    line_15,
};

/**
 * \brief Enumeration of exti trigger types
 */
enum class trigger : unsigned {
    none = 0,
    rising,
    falling,
    both,
};

/****************************** Function Declarations ************************************/
/**
 * \brief Registers an external interrupt on a specific pin/port
 * 
 * \param port The port to use
 * \param pin The pin to use
 * \param trigger Exti trigger type
 */
void register_external_interrupt(gpio::port port, gpio::pin_id pin, trigger trigger);

/**
 * \brief Clear a pending external interrupt on a specific line
 * 
 * \param line The interrupt line to clear
 */
void clear_external_interrupt_pending(line line);

};  // namespace HAL::exti
