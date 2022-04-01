/*! \file debug_port.c
*
*  \brief class for managing the debug port.
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "debug_port.hpp"
#include "hal_gpio.hpp"
#include "hal_nvic.hpp"
#include "hal_rcc.hpp"

/******************************* Global Variables **************************************/
using namespace HAL::gpio;

DebugPort<256> debug_port(USART3,
                          HAL::gpio::alternate_mode_pin{GPIOB,
                                                        HAL::rcc::ahb1_clocks::gpio_b,
                                                        pin_id::pin_10,
                                                        pin_mode::alternate,
                                                        pin_speed::very_high,
                                                        pull_mode::pull_up,
                                                        output_mode::push_pull,
                                                        alternate_mode::af7},
                          HAL::gpio::alternate_mode_pin{GPIOB,
                                                        HAL::rcc::ahb1_clocks::gpio_b,
                                                        pin_id::pin_11,
                                                        pin_mode::alternate,
                                                        pin_speed::very_high,
                                                        pull_mode::pull_up,
                                                        output_mode::push_pull,
                                                        alternate_mode::af7});


/**
 * \brief Interrupt handler for the USART3 peripheral, which is used as the debug port
 */
void usart3_irqn() {    
    // Handle RX Interrupt    
    auto rx_data_available = debug_port.read_status_register(HAL::usart::status_register::receive_data_available);
    auto rx_interrupt_enabled = debug_port.read_control_register(HAL::usart::control_register_1::receive_interrupt_enable) > 0;
    if (rx_data_available && rx_interrupt_enabled) {        
        debug_port.m_rx_buffer.push_back(static_cast<char>(debug_port.m_peripheral->DR & 0xFFu));
    }

    // Handle TX Interrupt
    auto tx_data_empty = debug_port.read_status_register(HAL::usart::status_register::transmit_data_empty);
    auto tx_interrupt_enabled = debug_port.read_control_register(HAL::usart::control_register_1::transmit_interrupt_enable) > 0;
    if (tx_data_empty && tx_interrupt_enabled) {
        auto value_to_send = debug_port.m_tx_buffer.pop_back();
        if (value_to_send.has_value()) {
            auto value = value_to_send.value();
            debug_port.m_peripheral->DR = value;
        }

        if (debug_port.m_tx_buffer.empty()) {
            debug_port.write_control_register(HAL::usart::control_register_1::transmit_interrupt_enable, 0x00);
        }
    }
}