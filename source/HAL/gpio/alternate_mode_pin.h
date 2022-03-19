/**
 * \file alternate_mode_pin.h
 * \author Graham Riches
 * \brief GPIO alternate mode pin
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
 * \brief class type for an alternate mode pin
 */
class alternate_mode_pin final : protected pin_base {
  public:
    
    /**
     * \brief Construct a new alternate mode pin object
     * 
     * \param bank Peripheral address pointer
     * \param pin The pin id to configure
     * \param mode Pin mode to use
     * \param speed Pin speed
     * \param pull_mode Pin pull mode
     * \param output_mode Output mode 
     * \param af_mode Alternate pin mode 
     */
    explicit alternate_mode_pin(GPIO_TypeDef* bank,
                       pin_id pin,
                       pin_mode mode,
                       pin_speed speed,
                       pull_mode pull_mode,
                       output_mode output_mode,
                       alternate_mode af_mode)
        : pin_base(bank, pin, mode, speed, pull_mode, output_mode) {
        for ( unsigned i = 0; i < TOTAL_PINS; i++ ) {            
            if ( ((0x01ul << i) & static_cast<uint32_t>(pin)) == (0x01ul << i) ) {
                // Select the appropriate register (high/low) from the AFR array
                uint8_t afr_array_index = (i < 8) ? 0 : 1;

                // Clear the register and set it to the appropriate value
                clear_bits(m_bank->AFR[afr_array_index], 0x0F << (i % 8) * 4);
                set_bits(m_bank->AFR[afr_array_index], static_cast<uint8_t>(af_mode) << ((i % 8) * 4));
                break;
            }
        }
    }
};

};  // namespace HAL::gpio