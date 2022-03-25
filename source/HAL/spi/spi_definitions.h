/**
 * \file spi_definitions.h
 * \author Graham Riches
 * \brief Definitions relevant for the SPI module
 * \version 0.1
 * \date 2022-03-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

namespace HAL::spi
{
/************************************ Types ********************************************/
/**
 * \brief Bit offsets for SPI control register 1
 */
enum class control_register_1 : unsigned {
    clock_phase = 0,
    clock_polarity = 1,
    master_select = 2,
    baudrate = 3,
    spi_enable = 6,
    lsb_first = 7,
    internal_slave_select = 8,
    software_slave_management = 9,
    receive_only = 10,
    data_frame_format = 11,
    crc_next = 12,
    crc_enable = 13,
    bidirectional_mode_select = 14,
    bidirectional_mode_enable = 15
};

/**
 * \brief Bit offsets for SPI control register 2 
 */
enum class control_register_2 : unsigned {
    receive_dma_enable = 0,
    transmit_dma_enable = 1,
    slave_select_output_enable = 2,
    frame_format = 4,
    error_interrupt_enable = 5,
    receive_interrupt_enable = 6,
    transmit_interrupt_enable = 7
};

/**
 * \brief Enumeration of bit offsets for the SPI status register 
 */
enum class status_register : unsigned {
    receive_data_available = 0,
    transmit_data_empty = 1,
    channel_side = 2,
    underrun_error = 3,
    crc_error = 4,
    mode_fault = 5,
    overrun_error = 6,
    busy = 7,
    frame_format_error = 8
};

/**
 * \brief Enumeration of baudrate scalers for SPI 
 */
enum class baudrate_prescaler : unsigned {
    prescaler_2 = 0b000,
    prescaler_4 = 0b001,
    prescaler_8 = 0b010,
    prescaler_16 = 0b011,
    prescaler_32 = 0b100,
    prescaler_64 = 0b101,
    prescaler_128 = 0b110,
    prescaler_256 = 0b111
};
};  // namespace HAL::spi