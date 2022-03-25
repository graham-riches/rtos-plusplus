/**
 * \file spi_polling.h
 * \author Graham Riches
 * \brief Implementation of a blocking polling SPI driver
 * \version 0.1
 * \date 2022-03-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include "spi_base.h"

namespace HAL::spi
{
class spi_polling : protected spi_base {
  public:
    /**
     * \brief Construct a new Polling SPI instance
     * 
     * \param spi_peripheral_address Address of the peripheral
     * \param chip_select Chip select pin
     */
    spi_polling(SPI_TypeDef* spi_peripheral_address, gpio::output_pin chip_select)
        : spi_base(spi_peripheral_address, chip_select) { }

    /**
     * \brief Read and write data at the same time (full duplex)
     * 
     * \param tx_buffer Transmit buffer to send
     * \param rx_buffer Receive buffer to fill
     * \param size Number of bytes to send
     */
    void read_write(uint8_t* tx_buffer, uint8_t* rx_buffer, std::size_t size);

    /**
     * \brief Read a block of data
     * 
     * \param rx_buffer Buffer to read data into
     * \param size Number of bytes to read
     */
    void read(uint8_t* rx_buffer, std::size_t size);

    /**
     * \brief Write a block of data
     * 
     * \param tx_buffer Buffer to write data from
     * \param size Number of bytes to write
     */
    void write(uint8_t* tx_buffer, std::size_t size);
};
};  // namespace HAL::spi