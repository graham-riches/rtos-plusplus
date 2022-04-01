/**
 * \file usart_base.cpp
 * \author Graham Riches
 * \brief Definitions for base USART driver
 * \version 0.1
 * \date 2022-03-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "hal_usart.hpp"
#include "hal_utilities.hpp"
#include <utility>

namespace HAL::usart
{
// Constructor
usart_base::usart_base(USART_TypeDef* usart, HAL::gpio::alternate_mode_pin&& tx_pin, HAL::gpio::alternate_mode_pin&& rx_pin)
    : m_peripheral(usart)
    , m_tx_pin(std::move(tx_pin))
    , m_rx_pin(std::move(rx_pin)) { }

// Read the status register
uint8_t usart_base::read_status_register(status_register reg) {
    return static_cast<uint8_t>(m_peripheral->SR & (0x01u << static_cast<uint8_t>(reg)));    
}

// Write CR1
void usart_base::write_control_register(control_register_1 reg, uint8_t value) {
    clear_bits(m_peripheral->CR1, 0x01u << static_cast<uint8_t>(reg));
    set_bits(m_peripheral->CR1, (value & 0x01u) << static_cast<uint8_t>(reg));
}

// Write CR2
void usart_base::write_control_register(control_register_2 reg, uint8_t value) {
    clear_bits(m_peripheral->CR2, 0x01u << static_cast<uint8_t>(reg));
    set_bits(m_peripheral->CR2, (value & 0x01u) << static_cast<uint8_t>(reg));
}

// Write CR3
void usart_base::write_control_register(control_register_3 reg, uint8_t value) {
    clear_bits(m_peripheral->CR3, 0x01u << static_cast<uint8_t>(reg));
    set_bits(m_peripheral->CR3, (value & 0x01u) << static_cast<uint8_t>(reg));
}

// Read CR1
uint8_t usart_base::read_control_register(control_register_1 reg) {
    uint8_t register_mask = 0b1;
    uint8_t register_value = static_cast<uint8_t>(m_peripheral->CR1 & (register_mask << static_cast<uint8_t>(reg)));
    return register_value;
}

// Read CR2
uint8_t usart_base::read_control_register(control_register_2 reg) {
    uint8_t register_mask = 0b1;
    uint8_t register_value = static_cast<uint8_t>(m_peripheral->CR2 & (register_mask << static_cast<uint8_t>(reg)));
    return register_value;
}

// Read CR3
uint8_t usart_base::read_control_register(control_register_3 reg) {
    uint8_t register_mask = 0b1;
    uint8_t register_value = static_cast<uint8_t>(m_peripheral->CR3 & (register_mask << static_cast<uint8_t>(reg)));
    return register_value;
}

// Set baudrate
void usart_base::set_baudrate(rcc::clocks clock, uint32_t baudrate) {
    uint32_t base_clock_rate = rcc::get_clock_speed(clock);
    uint32_t usart_div = (100 * base_clock_rate) / (16 * baudrate);
    uint32_t integer_component = usart_div / 100;
    uint32_t fractional_temp = usart_div - (100 * integer_component);

    // round the fractional component
    uint32_t fractional_component = (((fractional_temp * 16) + 50) / 100);

    // build out the register value and write it
    uint32_t register_value = (integer_component << 4) | (fractional_component & 0x0F);
    m_peripheral->BRR = static_cast<uint16_t>(register_value);
}
};  // namespace HAL::usart
