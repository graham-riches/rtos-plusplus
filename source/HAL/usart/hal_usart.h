/*! \file hal_usart.h
*
*  \brief hal_usart module functions and variables declarations.
*
*
*  \author Graham Riches
*/

#pragma once

/********************************** Includes *******************************************/
#include "hal_gpio.h"
#include "hal_nvic.h"
#include "hal_rcc.h"
#include "stm32f4xx.h"
#include <cstdint>

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

/**
 * \brief Base class for uart peripherals
 */
class usart_base {
    public:
    /**
     * \brief Construct a new usart base object
     * 
     * \param usart Peripheral address
     */
    explicit usart_base(USART_TypeDef* usart);

    //!< Delete copies and default construction
    usart_base(const usart_base&) = delete;
    usart_base& operator=(const usart_base&) = delete;
    usart_base() = delete;

    //!< Allow moves
    usart_base(usart_base&&) = default;    
    usart_base& operator=(usart_base&&) = default;


    /**
     * \brief Interface method to initialize the peripheral
     */
    virtual void initialize() = 0;

    /**
     * \brief Read the status register
     * 
     * \param reg Register to read
     * \return uint8_t Register valu
     */
    uint8_t read_status_register(status_register reg);

    /**
     * \brief Write to CR1
     * 
     * \param reg Register to write
     * \param value Value to write
     */
    void write_control_register(control_register_1 reg, uint8_t value);

    /**
     * \brief Write to CR2
     * 
     * \param reg Register to write
     * \param value Value to write
     */
    void write_control_register(control_register_2 reg, uint8_t value);

    /**
     * \brief Write to CR3
     * 
     * \param reg Register to write
     * \param value Value to write
     */
    void write_control_register(control_register_3 reg, uint8_t value);

    /**
     * \brief Read CR1
     * 
     * \param reg Register to read
     * \return uint8_t Register value
     */
    uint8_t read_control_register(control_register_1 reg);

    /**
     * \brief Read CR2
     * 
     * \param reg Register to read
     * \return uint8_t Register value
     */
    uint8_t read_control_register(control_register_2 reg);

    /**
     * \brief Read CR3
     * 
     * \param reg Register to read
     * \return uint8_t Register value
     */
    uint8_t read_control_register(control_register_3 reg);

    /**
     * \brief Set the baudrate obje
     * 
     * \param clock RCC clock to use (depends on peripheral)
     * \param baudrate Baudrate to use in bps
     */
    void set_baudrate(rcc::clocks clock, uint32_t baudrate);

    protected:
    USART_TypeDef* m_peripheral;
};

};  // namespace HAL
