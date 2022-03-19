/**
 * \file output_pin.h
 * \author Graham Riches
 * \brief GPIO output pin
 * \version 0.1
 * \date 2022-03-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include "gpio_definitions.h"
#include "hal_utilities.h"
#include "pin_base.h"

namespace HAL::gpio
{

/**
 * \brief class type for a GPIO output pin
 */
class output_pin final : protected pin_base {
  public:
    /**
     * \brief Construct a new output pin object
     * 
     * \param bank Peripheral address pointer
     * \param pin The pin id to configure
     * \param mode Pin mode to use
     * \param speed Pin speed
     * \param pull_mode Pin pull mode
     * \param output_mode Output mode     
     */
    explicit output_pin(GPIO_TypeDef* bank,
                                  pin_id pin,
                                  pin_mode mode,
                                  pin_speed speed,
                                  pull_mode pull_mode,
                                  output_mode output_mode)
        : pin_base(bank, pin, mode, speed, pull_mode, output_mode)
        , m_pin(pin) { }

    //!< Disable moves, copies, and default construction
    output_pin() = delete;

    /**
     * \brief Set the output pin state
     * 
     * \param high True to set the pin logic high
     */
    void set(bool high) {
        auto pin_mask = static_cast<uint16_t>(m_pin);
        if ( high ) {
            set_bits(m_bank->BSRRL, pin_mask);
        } else {
            set_bits(m_bank->BSRRH, pin_mask);
        }
    }

    /**
     * \brief Toggle the output pin state
     */
    void toggle() {
        auto pin_mask = static_cast<uint16_t>(m_pin);
        m_bank->ODR = m_bank->ODR ^ pin_mask;
    }

  private:
    pin_id m_pin;
};
};  // namespace HAL::gpio