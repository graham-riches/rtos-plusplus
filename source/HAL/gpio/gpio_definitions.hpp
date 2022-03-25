/**
 * \file gpio_definitions.hpp
 * \author Graham Riches
 * \brief Enumerations/definitions for gpio pins
 * \version 0.1
 * \date 2022-03-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once

/********************************** Includes *******************************************/
#include "stm32f4xx.h"

namespace HAL::gpio
{
/************************************ Constants ********************************************/
constexpr uint32_t TOTAL_PINS = 16;  //!< Total pin count per GPIO bank

/************************************ Types ********************************************/
/**
 * \brief Enumeration of GPIO pin modes
 */
enum class pin_mode : unsigned {
    input = 0,  //!< Pin is an input
    output,     //!< Pin is an output
    alternate,  //!< Pin is using an alternate function (e.g. uart, spi, etc.)
    analog      //!< Pin is an analog pin
};

/**
 * \brief GPIO pin output modes if the pin is configured as an output
 */
enum class output_mode : unsigned {
    push_pull = 0x00,  //!< configure the output in push pull mode
    open_drain = 0x01  //!< configure the output in open drain mode
};

/**
 * \brief GPIO switching speeds     
 */
enum class pin_speed : unsigned { low = 0, medium, high, very_high };

/**
 * \brief GPIO pull mode
 */
enum class pull_mode : unsigned {
    none = 0x00,  //!< no pull up or pull down activated
    pull_up,      //!< internal pull up activated
    pull_down     //!< internal pull down activated
};

/**
 * \brief Specification for GPIO pin alternate mode, which are used for other peripherals
 */
enum class alternate_mode : unsigned {
    af0 = 0,
    af1 = 1,
    af2 = 2,
    af3 = 3,
    af4 = 4,
    af5 = 5,
    af6 = 6,
    af7 = 7,
    af8 = 8,
    af9 = 9,
    af10 = 10,
    af11 = 11,
    af12 = 12,
    af13 = 13,
    af14 = 14,
    af15 = 15
};

/**
 * \brief Enumeration of GPIO pins by bit
 */
enum class pin_id : unsigned {
    pin_0 = 0x0001,
    pin_1 = 0x0002,
    pin_2 = 0x0004,
    pin_3 = 0x0008,
    pin_4 = 0x0010,
    pin_5 = 0x0020,
    pin_6 = 0x0040,
    pin_7 = 0x0080,
    pin_8 = 0x0100,
    pin_9 = 0x0200,
    pin_10 = 0x0400,
    pin_11 = 0x0800,
    pin_12 = 0x1000,
    pin_13 = 0x2000,
    pin_14 = 0x4000,
    pin_15 = 0x8000,
    all = 0xFFFF
};

/**
 * \brief Gpio ports
 * \note All pin numbers with the same number (i.e. PA0, PB0) share a single
 *       interrupt line to the processor
 */
enum class port : unsigned {
    port_a = 0,
    port_b,
    port_c,
    port_d,
    port_e,
    port_f,
    port_g,
    port_h,
    port_i,
};

};  // namespace HAL::gpio