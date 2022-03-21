/**
 * \file pin_base.h
 * \author Graham Riches
 * \brief GPIO pin base class
 * \version 0.1
 * \date 2022-03-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

/****************************** Includes ***********************************/
#include "gpio_definitions.h"
#include "hal_rcc.h"
#include "hal_utilities.h"

namespace HAL::gpio
{
/****************************** Constants ***********************************/
constexpr uint32_t CLEAR_MODE_REGISTER_MASK = 0x03;       //!< Bitmask to clear the mode register for a pin
constexpr uint32_t CLEAR_SPEED_REGISTER_MASK = 0x03;      //!< Bitmask to clear the speed register for a pin
constexpr uint32_t CLEAR_PORT_TYPE_REGISTER_MASK = 0x01;  //!< Bitmask to clear the port type register for a pin
constexpr uint32_t CLEAR_PULLUP_REGISTER_MASK = 0x03;     //!< Bitmask to clear the pullup/pulldown config register for a pin

/****************************** Types ***********************************/
/**
 * \brief Base GPIO pin class, which sets up the core registers for the pin
 */
class pin_base {
  public:
    /**
     * \brief Construct a new pin base object
     * 
     * \param bank Peripheral address pointer
     * \param peripheral_clock AHB1 clock enumeration
     * \param pin The pin id to configure
     * \param mode Pin mode to use
     * \param speed Pin speed
     * \param pull_mode Pin pull mode
     * \param output_mode Output mode
     */
    explicit pin_base(GPIO_TypeDef* bank,
                      AHB1Clocks peripheral_clock,
                      pin_id pin,
                      pin_mode mode,
                      pin_speed speed,
                      pull_mode pull_mode,
                      output_mode output_mode)
        : m_bank(bank) {            
        // Enable GPIO clock
        reset_control_clock.set_ahb_clock(peripheral_clock, true);
        
        // Find the pin and setup the common registers
        for ( unsigned i = 0; i < TOTAL_PINS; i++ ) {
            if (is_bit_set(static_cast<uint32_t>(pin), 0x01ul << i)) {            
                // Pin mode
                clear_bits(m_bank->MODER, CLEAR_MODE_REGISTER_MASK << (2 * i));
                set_bits(m_bank->MODER, static_cast<uint32_t>(mode) << (2 * i));

                // Set the pin port type (push_pull/open_drain)
                clear_bits(m_bank->OTYPER, CLEAR_PORT_TYPE_REGISTER_MASK << i);
                set_bits(m_bank->OTYPER, static_cast<uint32_t>(output_mode) << i);

                // Set the speed and push pull config only for outputs and alternate function pins
                if ( (mode == pin_mode::output) || (mode == pin_mode::alternate) ) {
                    clear_bits(m_bank->OSPEEDR, CLEAR_SPEED_REGISTER_MASK << (2 * i));
                    set_bits(m_bank->OSPEEDR, static_cast<uint32_t>(speed) << (2 * i));
                    clear_bits(m_bank->PUPDR, CLEAR_PULLUP_REGISTER_MASK << (2 * i));
                    set_bits(m_bank->PUPDR, static_cast<uint32_t>(pull_mode) << (2 * i));
                }
                break;
            }
        }
    }

    //!< Disable moves, copies, and default construction
    pin_base() = delete;
    pin_base(const pin_base&) = delete;
    pin_base(pin_base&&) = delete;
    pin_base& operator=(const pin_base&) = delete;
    pin_base& operator=(pin_base&&) = delete;

    //!< Default destructor
    ~pin_base() = default;

  protected:
    GPIO_TypeDef* m_bank;
};

};  // namespace HAL::gpio