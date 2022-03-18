/*! \file hal_usart.c
*
*  \brief HAL++ implementation of the STM32 USART peripherals.
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "hal_usart.h"
#include <cassert>
#include <string.h>


namespace HAL
{
/****************************** Function Definitions ***********************************/

USARTBase::USARTBase(USART_TypeDef* usart) {
    assert(usart != nullptr);
    peripheral = usart;
}

uint8_t USARTBase::read_status_register(USARTStatusRegister reg) {
    return static_cast<uint8_t>(peripheral->SR & (0x01u << static_cast<uint8_t>(reg)));
}

void USARTBase::write_control_register(USARTControlRegister1 reg, uint8_t value) {
    peripheral->CR1 = peripheral->CR1 & ~static_cast<uint16_t>((static_cast<uint8_t>(0x01) << static_cast<uint8_t>(reg)));
    peripheral->CR1 = peripheral->CR1 | static_cast<uint16_t>(((value & 0x01) << static_cast<uint8_t>(reg)));
}

void USARTBase::write_control_register(USARTControlRegister2 reg, uint8_t value) {    
    peripheral->CR2 &= ~static_cast<uint16_t>((static_cast<uint8_t>(0x01) << static_cast<uint8_t>(reg)));
    
    peripheral->CR2 |= static_cast<uint16_t>(((value & 0x01) << static_cast<uint8_t>(reg)));
}

void USARTBase::write_control_register(USARTControlRegister3 reg, uint8_t value) {    
    peripheral->CR3 &= ~static_cast<uint16_t>((static_cast<uint8_t>(0x01) << static_cast<uint8_t>(reg)));    
    peripheral->CR3 |= static_cast<uint16_t>(((value & 0x01) << static_cast<uint8_t>(reg)));
}

uint8_t USARTBase::read_control_register(USARTControlRegister1 reg) {
    uint8_t register_mask = 0b1;
    uint8_t register_value = static_cast<uint8_t>(peripheral->CR1 & (register_mask << static_cast<uint8_t>(reg)));
    return register_value;
}

uint8_t USARTBase::read_control_register(USARTControlRegister2 reg) {
    uint8_t register_mask = 0b1;
    uint8_t register_value = static_cast<uint8_t>(peripheral->CR2 & (register_mask << static_cast<uint8_t>(reg)));
    return register_value;
}

uint8_t USARTBase::read_control_register(USARTControlRegister3 reg) {
    uint8_t register_mask = 0b1;
    uint8_t register_value = static_cast<uint8_t>(peripheral->CR3 & (register_mask << static_cast<uint8_t>(reg)));
    return register_value;
}

/**
 * \brief Set the baudrate for a usart peripheral
 * 
 * \param clock enum value of which peripheral clock the usart is associated with
 * \param baudrate baudrate to set
 * \todo remove base clock rate and get that direct from HAL_rcc
 * \todo account for oversampling mode
 */
void USARTBase::set_baudrate(HAL::Clocks clock, uint32_t baudrate) {
    uint32_t base_clock_rate = HAL::reset_control_clock.get_clock_speed(clock);
    uint32_t usart_div = (100 * base_clock_rate) / (16 * baudrate);    
    uint32_t integer_component = usart_div / 100;    
    uint32_t fractional_temp = usart_div - (100 * integer_component);

    // round the fractional component
    uint32_t fractional_component = (((fractional_temp * 16) + 50) / 100);

    // build out the register value and write it
    uint32_t register_value = (integer_component << 4) | (fractional_component & 0x0F);    
    peripheral->BRR = static_cast<uint16_t>(register_value);
}

/**
 * \brief handle an interrupt on a usart peripheral
 * \param type type of the interrupt
 */
void USARTInterrupt::irq_handler(uint8_t type) {
    PARAMETER_NOT_USED(type);

    // Handle RX interrupts
    bool rx_data_available = read_status_register(USARTStatusRegister::receive_data_available);
    bool rx_interrupt_enabled = read_control_register(USARTControlRegister1::receive_interrupt_enable);

    if ( (rx_data_available) && (rx_interrupt_enabled) ) {
        uint8_t data = static_cast<uint8_t>(peripheral->DR & 0xFF);
        rx_buffer.push_back(data);
    }

    // Handle any tx interrupts
    bool tx_data_empty = read_status_register(USARTStatusRegister::transmit_data_empty);
    bool tx_interrupt_enabled = read_control_register(USARTControlRegister1::transmit_interrupt_enable);

    if ( (tx_data_empty) && (tx_interrupt_enabled) ) {
        auto maybe_data = tx_buffer.pop_front();
        if (maybe_data.has_value()) {
            peripheral->DR = maybe_data.value();
        }

        // If there is no more data to send, disable the interrupt
        if ( tx_buffer.empty() ) {
            write_control_register(USARTControlRegister1::transmit_interrupt_enable, 0x00);
        }
    }
}

void USARTInterrupt::send(uint8_t* data, uint16_t size) {    
    while ( size-- ) {
        tx_buffer.push_back(*data++);
        if ( tx_buffer.full() ) {
            break;
        }
    }
    
    write_control_register(USARTControlRegister1::transmit_interrupt_enable, 0x01);
}

void USARTInterrupt::send(const char* data) {    
    uint16_t size = strlen(data);

    for ( uint16_t i = 0; i < size; i++ ) {
        uint8_t dataByte = (uint8_t)data[i];
        tx_buffer.push_back(dataByte);

        if ( tx_buffer.full() ) {
            break;
        }
    }
    
    write_control_register(USARTControlRegister1::transmit_interrupt_enable, 0x01);
}

void USARTInterrupt::send(const char data) {
    tx_buffer.push_back(static_cast<uint8_t>(data));    
    write_control_register(USARTControlRegister1::transmit_interrupt_enable, 0x01);
}

};  // namespace HAL
