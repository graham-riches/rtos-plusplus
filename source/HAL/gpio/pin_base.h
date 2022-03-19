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

#include "gpio_definitions.h"

namespace HAL::gpio
{

/**
 * \brief Base GPIO pin class, which sets up the core registers for the pin
 */
class pin_base {
  public:
    /**
     * \brief Construct a new pin base object
     * 
     * \param bank Peripheral address pointer
     * \param pin The pin id to configure
     * \param mode Pin mode to use
     * \param speed Pin speed
     * \param pull_mode Pin pull mode
     * \param output_mode Output mode
     */
    explicit pin_base(GPIO_TypeDef* bank,
                                pin_id pin,
                                pin_mode mode,
                                pin_speed speed,
                                pull_mode pull_mode,
                                output_mode output_mode);

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