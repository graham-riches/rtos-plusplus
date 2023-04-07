// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Graham Riches

#include "spi_base.hpp"
#include "hal_utilities.hpp"

namespace HAL::spi
{
//!< Constructor
spi_base::spi_base(SPI_TypeDef* spi_peripheral_address, gpio::output_pin& chip_select)
: m_peripheral(spi_peripheral_address)
, m_chip_select(&chip_select) {    
    // Pull the chip select high by default
    m_chip_select->set(true);
}

//!< Read the SPI status register
uint8_t spi_base::read_status_register(status_register reg) {
    return static_cast<uint8_t>(m_peripheral->SR & (0x01u << static_cast<uint8_t>(reg)));
}

//!< Write CR1
void spi_base::write_control_register(control_register_1 reg, uint8_t value) {
    // Clear a single bit unless the register is the baudrate (3 bits )
    uint8_t register_mask = (reg != control_register_1::baudrate) ? 0b1 : 0b111;    
    clear_bits(m_peripheral->CR1, (register_mask << static_cast<uint8_t>(reg)));
    set_bits(m_peripheral->CR1, (value & register_mask) << static_cast<uint8_t>(reg));    
}

//!< Write CR2
void spi_base::write_control_register(control_register_2 reg, uint8_t value) {
    clear_bits(m_peripheral->CR2, 0x01u << static_cast<uint8_t>(reg));
    set_bits(m_peripheral->CR2, (value & 0x01) << static_cast<uint8_t>(reg));    
}

//!< Read CR1
uint8_t spi_base::read_control_register(control_register_1 reg) {
    uint8_t register_mask = (reg != control_register_1::baudrate) ? 0b1 : 0b111;
    uint8_t register_value = static_cast<uint8_t>(m_peripheral->CR1 & (register_mask << static_cast<uint8_t>(reg)));
    return register_value;
}

//!< Read CR2
uint8_t spi_base::read_control_register(control_register_2 reg) {
    uint8_t register_mask = 0b1;
    uint8_t register_value = static_cast<uint8_t>(m_peripheral->CR2 & (register_mask << static_cast<uint8_t>(reg)));
    return register_value;
}

//!< Set the baud rate
void spi_base::set_baudrate(baudrate_prescaler prescaler) {
    write_control_register(control_register_1::baudrate, static_cast<uint8_t>(prescaler));
}
};