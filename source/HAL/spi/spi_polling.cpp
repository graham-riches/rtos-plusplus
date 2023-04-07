// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Graham Riches

/********************************** Includes *******************************************/
#include "spi_polling.hpp"

namespace HAL::spi
{
// Read/Write data
void spi_polling::read_write(uint8_t* tx_buffer, uint8_t* rx_buffer, std::size_t size) {
    m_chip_select->set(false);

    while ( size ) {
        // Wait for the transmit buffer to clear
        while ( read_status_register(status_register::transmit_data_empty) == false ) {
        }

        // put data outgoing into the data register if sending, or junk if receiving
        m_peripheral->DR = (tx_buffer != nullptr) ? *tx_buffer++ : 0x00u;

        // wait for the receive buffer to clear
        while ( read_status_register(status_register::receive_data_available) == false ) {
        }

        uint8_t rx_data = static_cast<uint8_t>(m_peripheral->DR);

        // receive data
        if ( rx_buffer != nullptr ) {
            *rx_buffer++ = rx_data;
        }

        size--;
    }
    m_chip_select->set(true);
}

// Read data helper
void spi_polling::read(uint8_t* rx_buffer, std::size_t size) {
    read_write(nullptr, rx_buffer, size);
}

// Write data helper
void spi_polling::write(uint8_t* tx_buffer, std::size_t size) {
    read_write(tx_buffer, nullptr, size);
}
};  // namespace HAL::spi
