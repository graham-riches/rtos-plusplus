// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Graham Riches

/********************************** Includes *******************************************/
#include "hal_exti.hpp"
#include "hal_rcc.hpp"
#include "hal_utilities.hpp"

namespace HAL::exti
{
/**
 * \brief register an external interrupt
 * 
 * \param port which port to register the interrupt on
 * \param pin the pin
 * \param trigger the trigger for the external interrupt (rising edge, etc)
 * \details SYSCFG has an array of 4 32-bit values which contain the configuration of the 16 EXTI lines.
 *          Each EXTI port configuration is 4-bits at the lower 16-bits of each register in the array.
 *          The GPIO pins are inconveniently bit flags, which makes counting the actual value annoying BUT
 *          there is a solution :)
 *          
 */
void register_external_interrupt(gpio::port port, gpio::pin_id pin, trigger trigger) {
    rcc::set_apb_clock(rcc::apb2_clocks::sys_config, true);
    auto pin_offset = static_cast<uint8_t>(pin);

    uint8_t pin_number = 1;
    while ( pin_offset >>= 1 ) {
        pin_number++;
    }

    // Setup interrupt
    auto exti_index = pin_number / 4;
    auto exti_offset = (pin_number - 1) % 4;
    clear_bits(SYSCFG->EXTICR[exti_index], 0xFFu << (exti_offset * 4));
    set_bits(SYSCFG->EXTICR[exti_index], static_cast<uint8_t>(port) << (4 * exti_offset));

    // Clear interrupt mask
    uint32_t pin_mask = (0x01 << (pin_number - 1));
    set_bits(EXTI->IMR, pin_mask);

    // Set trigger mode
    switch ( trigger ) {
        case trigger::none:
            clear_bits(EXTI->FTSR, pin_mask);
            clear_bits(EXTI->RTSR, pin_mask);
            break;

        case trigger::rising:
            clear_bits(EXTI->FTSR, pin_mask);
            set_bits(EXTI->RTSR, pin_mask);
            break;

        case trigger::falling:
            clear_bits(EXTI->RTSR, pin_mask);
            set_bits(EXTI->FTSR, pin_mask);
            break;

        case trigger::both:
            set_bits(EXTI->FTSR, pin_mask);
            set_bits(EXTI->RTSR, pin_mask);
            break;

        default:
            break;
    }
};

// Clean an interrupt line
void clear_external_interrupt_pending(line line) {
    set_bits(EXTI->PR, 0x01 << static_cast<uint8_t>(line));
}

}  // namespace HAL::exti
