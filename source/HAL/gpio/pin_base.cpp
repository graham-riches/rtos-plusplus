/**
 * \file pin_base.cpp
 * \author Graham Riches
 * \brief Base GPIO functionality
 * \version 0.1
 * \date 2022-03-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/********************************** Includes *******************************************/
#include "pin_base.h"
#include "hal_rcc.h"
#include "hal_utilities.h"
#include <array>
#include <utility>

namespace HAL::gpio
{

/****************************** Constants ***********************************/
constexpr uint32_t CLEAR_MODE_REGISTER_MASK      = 0x03;   //!< Bitmask to clear the mode register for a pin
constexpr uint32_t CLEAR_SPEED_REGISTER_MASK      = 0x03;  //!< Bitmask to clear the speed register for a pin
constexpr uint32_t CLEAR_PORT_TYPE_REGISTER_MASK  = 0x01;  //!< Bitmask to clear the port type register for a pin
constexpr uint32_t CLEAR_PULLUP_REGISTER_MASK     = 0x03;  //!< Bitmask to clear the pullup/pulldown config register for a pin

/****************************** Local Variables ***********************************/
static const std::array<std::pair<GPIO_TypeDef*, AHB1Clocks>, 9> gpio_clock_map{{
    {GPIOA, AHB1Clocks::gpio_a},
    {GPIOB, AHB1Clocks::gpio_b},
    {GPIOC, AHB1Clocks::gpio_c},
    {GPIOD, AHB1Clocks::gpio_d},
    {GPIOE, AHB1Clocks::gpio_e},
    {GPIOF, AHB1Clocks::gpio_f},
    {GPIOG, AHB1Clocks::gpio_g},
    {GPIOH, AHB1Clocks::gpio_h},
    {GPIOI, AHB1Clocks::gpio_i},
}};

/****************************** Function Definitions ***********************************/
pin_base::pin_base(GPIO_TypeDef* bank, pin_id pin, pin_mode mode, pin_speed speed, pull_mode pull_mode, output_mode output_mode)
    : m_bank(bank) {

    // Enable the peripheral clock. Note: this must be done before writing to the registers
    for (auto& peripheral_clock : gpio_clock_map) {
        if (bank == peripheral_clock.first) {
            reset_control_clock.set_ahb_clock(peripheral_clock.second, true);
            break;
        }
    }
        
    // Find the pin and setup the common registers
    for ( unsigned i = 0; i < TOTAL_PINS; i++ ) {        
        if ( (0x01 << i) & static_cast<uint32_t>(pin) == (0x01 << i) ) {            
            // Pin mode
            clear_bits(m_bank->MODER, CLEAR_MODE_REGISTER_MASK << (2 * i));
            set_bits(m_bank->MODER, static_cast<uint32_t>(mode) << (2 * i));

            // Set the pin port type (push_pull/open_drain)
            clear_bits(m_bank->OTYPER, CLEAR_PORT_TYPE_REGISTER_MASK << i);
            set_bits(m_bank->OTYPER, static_cast<uint32_t>(output_mode) << i);            

            // Set the speed and push pull config only for outputs and alternate function pins
            if ( (mode == pin_mode::output) || (mode == pin_mode::alternate) ) {            
                clear_bits(m_bank->OSPEEDR, CLEAR_SPEED_REGISTER_MASK << (2* i));
                set_bits(m_bank->OSPEEDR, static_cast<uint32_t>(speed) << (2 * i));
                clear_bits(m_bank->PUPDR, CLEAR_PULLUP_REGISTER_MASK << (2 * i));
                set_bits(m_bank->PUPDR, static_cast<uint32_t>(pull_mode) << (2 * i));                
            }
            break;
        }
    }
}
};  // namespace HAL
