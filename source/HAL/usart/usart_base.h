/**
 * \file usart_base.h
 * \author Graham Riches
 * \brief Usart base class for driver implementation
 * \version 0.1
 * \date 2022-03-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include "usart_definitions.h"
#include "hal_rcc.h"
#include "hal_gpio.h"
#include "stm32f4xx.h"

namespace HAL::usart
{
/**
 * \brief Base class for uart peripherals
 */
class usart_base {
    public:
    /**
     * \brief Construct a new usart base object
     * 
     * \param usart Peripheral address
     * \param tx_pin TX alternate mode pin
     * \param rx_pin RX alternate mode pin
     */
    explicit usart_base(USART_TypeDef* usart, HAL::gpio::alternate_mode_pin&& tx_pin, HAL::gpio::alternate_mode_pin&& rx_pin);

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
    HAL::gpio::alternate_mode_pin m_tx_pin;
    HAL::gpio::alternate_mode_pin m_rx_pin;
};
};