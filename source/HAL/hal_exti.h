/*! \file hal_exti.h
*
*  \brief external interrupts management for GPIO lines
*
*
*  \author Graham Riches
*/

#pragma once

/********************************** Includes *******************************************/
#include "common.h"
#include "hal_gpio.h"
#include "stm32f4xx.h"


namespace HAL
{

/*********************************** Consts ********************************************/

/************************************ Types ********************************************/
/**
 * \brief enumeration of EXTI lines
 */
enum class EXTILine : unsigned {
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
 * \brief enumeration for selecting exti source
 * \note all pin numbers with the same number (i.e. PA0, PB0) share a single
 *       interrupt line to the processor
 */
enum class EXTIPort : unsigned {
    gpio_port_a = 0,
    gpio_port_b,
    gpio_port_c,
    gpio_port_d,
    gpio_port_e,
    gpio_port_f,
    gpio_port_g,
    gpio_port_h,
    gpio_port_i,
};

/**
 * \brief enumeration of exti trigger types
 */
enum class EXTITrigger : unsigned {
    none = 0,
    rising,
    falling,
    both,
};

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/

/****************************** Functions Prototype ************************************/
void register_external_interrupt(EXTIPort port, Pins pin, EXTITrigger trigger);
void clear_external_interrupt_pending(EXTILine line);

};  // namespace HAL
