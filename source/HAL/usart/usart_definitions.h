/**
 * \file usart_definitions.h
 * \author Graham Riches
 * \brief USART definitions and enumerations
 * \version 0.1
 * \date 2022-03-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

namespace HAL::usart
{
/************************************ Types ********************************************/
/**
 * \brief bit offsets for USART status register
 */
enum class status_register : unsigned {
    parity_error = 0,
    framing_error = 1,
    noise_detected = 2,
    overrun_error = 3,
    idle_detected = 4,
    receive_data_available = 5,
    transmission_complete = 6,
    transmit_data_empty = 7,
    line_break_detected = 8,
    cts_flag = 9
};

/**
 * \brief bit offsets for USART control register one
 */
enum class control_register_1 : unsigned {
    send_break = 0,
    receiver_wakeup = 1,
    receiver_enable = 2,
    transmitter_enable = 3,
    idle_interrupt_enable = 4,
    receive_interrupt_enable = 5,
    transmit_complete_interrupt_enable = 6,
    transmit_interrupt_enable = 7,
    parity_error_interrupt_enable = 8,
    parity_selection = 9,
    parity_control_enable = 10,
    wake_method = 11,
    word_length = 12,
    usart_enable = 13,
    oversampling_enable = 15
};

/**
 * \brief bit offsets for USART control register two
 */
enum class control_register_2 : unsigned {
    address = 0,
    line_break_detection_length = 5,
    line_break_interrupt_enable = 6,
    last_bit_clock_pulse = 8,
    clock_phase = 9,
    clock_polarity = 10,
    clock_enable = 11,
    stop_bits = 12,
    line_mode_enable = 14,
};

/**
 * \brief bit offsets for USART control register three
 */
enum class control_register_3 : unsigned {
    error_interrupt_enable = 0,
    irda_mode_enable = 1,
    irda_low_power_mode = 2,
    half_duplex_enable = 3,
    smartcard_nack_enable = 4,
    smartcard_enable = 5,
    dma_receive_enable = 6,
    dma_transmit_enable = 7,
    rts_enable = 8,
    cts_enable = 9,
    cts_interrupt_enable = 10,
    onebit_mode_enable = 11,
};
};  // namespace HAL