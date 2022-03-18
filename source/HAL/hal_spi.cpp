/*! \file hal_spi.c
*
*  \brief HAL SPI interface.
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "hal_spi.h"
#include <cassert>

namespace HAL
{
/****************************** SPIBase Definitions ***********************************/

SPIBase::SPIBase(SPI_TypeDef* spi_peripheral_address, OutputPin chip_select) {
    assert(spi_peripheral_address != nullptr);
    peripheral = spi_peripheral_address;
    chip_select = chip_select;

    // Pull the chip select high by default
    chip_select.set(true);
}

uint8_t SPIBase::read_status_register(SPIStatusRegister reg) {
    return static_cast<uint8_t>(peripheral->SR & (0x01u << static_cast<uint8_t>(reg)));
}

void SPIBase::write_control_register(SPIControlRegister1 reg, uint8_t value) {
    // Clear a single bit unless the register is the baudrate (3 bits )
    uint8_t register_mask = (reg != SPIControlRegister1::baudrate) ? 0b1 : 0b111;    
    peripheral->CR1 = peripheral->CR1 & ~static_cast<uint16_t>((register_mask << static_cast<uint8_t>(reg)));
    peripheral->CR1 = peripheral->CR1 | static_cast<uint16_t>(((value & register_mask) << static_cast<uint8_t>(reg)));
}

void SPIBase::write_control_register(SPIControlRegister2 reg, uint8_t value) {    
    peripheral->CR2 = peripheral->CR2 & ~static_cast<uint16_t>((static_cast<uint8_t>(0x01) << static_cast<uint8_t>(reg)));
    peripheral->CR2 = peripheral->CR2 | static_cast<uint16_t>(((value & 0x01) << static_cast<uint8_t>(reg)));
}

uint8_t SPIBase::read_control_register(SPIControlRegister1 reg) {
    uint8_t register_mask = (reg != SPIControlRegister1::baudrate) ? 0b1 : 0b111;
    uint8_t register_value = static_cast<uint8_t>(peripheral->CR1 & (register_mask << static_cast<uint8_t>(reg)));
    return register_value;
}

uint8_t SPIBase::read_control_register(SPIControlRegister2 reg) {
    uint8_t register_mask = 0b1;
    uint8_t register_value = static_cast<uint8_t>(peripheral->CR2 & (register_mask << static_cast<uint8_t>(reg)));
    return register_value;
}

void SPIBase::set_baudrate(SPIBaudratePrescaler prescaler) {
    write_control_register(SPIControlRegister1::baudrate, static_cast<uint8_t>(prescaler));
}

/****************************** SPIPolling Definitions ***********************************/

void SPIPolling::read_write(uint8_t* tx_buffer, uint8_t* rx_buffer, uint16_t size) {     
    chip_select.set(false);
    
    while ( size  ) {
        // Wait for the transmit buffer to clear
        while ( read_status_register(SPIStatusRegister::transmit_data_empty) == false ) {
        }

        // put data outgoing into the data register if sending, or junk if receiving
        peripheral->DR = (tx_buffer != nullptr) ? *tx_buffer++ : static_cast<uint8_t>(0x00);

        // wait for the receive buffer to clear
        while ( read_status_register(SPIStatusRegister::receive_data_available) == false ) {
        }

        uint8_t rx_data = static_cast<uint8_t>(peripheral->DR);

        // receive data
        if ( rx_buffer != nullptr ) {
            *rx_buffer++ = rx_data;
        }

        size--;
    }    
    chip_select.set(true);
}

void SPIPolling::read(uint8_t* rx_buffer, uint16_t size) {
    read_write(nullptr, rx_buffer, size);
}

void SPIPolling::write(uint8_t* tx_buffer, uint16_t size) {
    read_write(tx_buffer, nullptr, size);
}

};  // namespace HAL
