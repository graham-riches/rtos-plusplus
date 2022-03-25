/*! \file debug_port.h
*
*  \brief debug_port module functions and variables declarations.
*
*
*  \author Graham Riches
*/

#pragma once

/********************************** Includes *******************************************/
#include "common.h"
#include "hal_gpio.h"
#include "hal_usart.h"
#include "hal_rcc.h"
#include "device_port.h"
#include "stm32f4xx.h"
#include "ring_buffer.h"
#include <cstdarg>
#include <utility>

/************************************ Types ********************************************/
// Debug UART
template <std::size_t PrintBufferSize>
class DebugPort : protected HAL::usart::usart_base {
  public:
    /**
     * \brief Construct a new Debug Port object
     * 
     * \param usart Peripheral base address
     */
    DebugPort(USART_TypeDef* usart, HAL::gpio::alternate_mode_pin&& tx_pin, HAL::gpio::alternate_mode_pin&& rx_pin)
        : HAL::usart::usart_base(usart, std::move(tx_pin), std::move(rx_pin)) { }

    /**
     * \brief Peripheral interface method to setup the peripheral    
     */
    void initialize() {        
        // Enable the GPIO clocks and the USART clocks 
        HAL::rcc::set_apb_clock(HAL::rcc::apb1_clocks::usart_3, true);

        // Configure USART settings
        write_control_register(HAL::usart::control_register_1::parity_selection, 0x00);
        write_control_register(HAL::usart::control_register_1::word_length, 0x00);
        write_control_register(HAL::usart::control_register_2::stop_bits, 0x00);
        write_control_register(HAL::usart::control_register_3::cts_enable, 0x00);
        write_control_register(HAL::usart::control_register_3::rts_enable, 0x00);
        set_baudrate(HAL::rcc::clocks::APB1, 115200);

        // Enable the usart and interrupts
        write_control_register(HAL::usart::control_register_1::receiver_enable, 0x01);
        write_control_register(HAL::usart::control_register_1::transmitter_enable, 0x01);
        write_control_register(HAL::usart::control_register_1::receive_interrupt_enable, 0x01);


        // Set the interrupt priority
        HAL::nvic::set_priority(stm32f4_irq::usart_3, HAL::nvic::isr_preemption_priority::level_2);        

        // Enable
        write_control_register(HAL::usart::control_register_1::usart_enable, 0x01);
    }

    /**
     * \brief Log a message
     * 
     * \param message The message to log
     * \param ... Variadic arguments
     */
    void log_message(const char* message, ...) {
        va_list args;
        va_start(args, message);
        auto bytes_to_write = vsnprintf(m_print_buffer, PrintBufferSize, message, args);
        va_end(args);
        // Push into the ring buffer
        for (unsigned i = 0; i < bytes_to_write; ++i) {
            m_rb.push_back(m_print_buffer[i]);
        }
        // Set TX interrupt flag
        write_control_register(HAL::usart::control_register_1::transmit_interrupt_enable, 0x01);
    }

  private:
    ring_buffer<char, PrintBufferSize> m_rb;
    char m_print_buffer[PrintBufferSize];


    // TODO: add interrupt handler routine for this peripheral here

};

/******************************* Global Variables **************************************/
extern DebugPort<128> debug_port;
