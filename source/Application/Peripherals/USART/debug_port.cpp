/*! \file debug_port.c
*
*  \brief class for managing the debug port.
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "debug_port.h"
#include "hal_gpio.h"
#include "hal_interrupt.h"
#include "hal_rcc.h"
#include <cstdio>
#include <cstring>

/******************************** Local Variables **************************************/
/* create the GPIO pins */
static HAL::AlternateModePin tx_pin(
    GPIOB, HAL::Pins::pin_10, HAL::PinMode::alternate, HAL::Speed::very_high, HAL::PullMode::pull_up, HAL::OutputMode::push_pull, HAL::AlternateMode::af7);
static HAL::AlternateModePin rx_pin(
    GPIOB, HAL::Pins::pin_11, HAL::PinMode::alternate, HAL::Speed::very_high, HAL::PullMode::pull_up, HAL::OutputMode::push_pull, HAL::AlternateMode::af7);

/******************************* Global Variables **************************************/
DebugPort debug_port(USART3);

/****************************** Functions Prototype ************************************/

/****************************** Functions Definition ***********************************/
/**
 * \brief Construct a new Debug Port:: Debug Port object
 * 
 * \param usart the uart peripheral address pointer
 * \param tx_size size of the tx ring buffer
 * \param rx_size size of the rx ring buffer
 */
DebugPort::DebugPort(USART_TypeDef* usart)
    : HAL::USARTInterrupt(usart)
    , print_buffer(std::make_unique<char[]>(128)) {
        //!< TODO: fix hard coded print buffer size!!
    print_buffer_size = 128;
}

/**
 * \brief initialize the debug port with the correct HW settings
 */
void DebugPort::initialize(void) {
    using namespace HAL;

    /* enable the GPIO clocks and the USART clocks */
    reset_control_clock.set_apb_clock(APB1Clocks::usart_3, true);

    /* configure the usart with the application specific settings */
    write_control_register(USARTControlRegister1::parity_selection, 0x00);
    write_control_register(USARTControlRegister1::word_length, 0x00);
    write_control_register(USARTControlRegister2::stop_bits, 0x00);
    write_control_register(USARTControlRegister3::cts_enable, 0x00);
    write_control_register(USARTControlRegister3::rts_enable, 0x00);
    set_baudrate(Clocks::APB1, 115200);

    /* enable the usart and interrupts */
    write_control_register(USARTControlRegister1::receiver_enable, 0x01);
    write_control_register(USARTControlRegister1::transmitter_enable, 0x01);
    write_control_register(USARTControlRegister1::receive_interrupt_enable, 0x01);

    /* register the interrupt in the hal interrupts table */
    interrupt_manager.register_callback(InterruptName::usart_3, this, 0, PreemptionPriority::level_2);

    /* enable the UART */
    write_control_register(USARTControlRegister1::usart_enable, 0x01);
}

/**
 * \brief send a formatted log message out
 * 
 * \param message the formatted message
 * \param tag logging tag
 * \param args variadic args pack
 */
void DebugPort::log_message(const char* message, const char* tag, va_list args) {
    send(tag);
    send(": ");
    vsnprintf(print_buffer.get(), print_buffer_size, message, args);
    send(print_buffer.get());
    send("\r\n");
}

/**
 * \brief log a debug message
 * 
 * \param message the message to send
 * \param ... variadic args
 */
void DebugPort::debug(const char* message, ...) {
    va_list args;
    va_start(args, message);
    log_message(message, "DEBUG", args);
    va_end(args);
}

/**
 * \brief log an info message
 * 
 * \param message the message to send
 * \param ... variadic print arguments
 */
void DebugPort::info(const char* message, ...) {
    va_list args;
    va_start(args, message);
    log_message(message, "INFO", args);
    va_end(args);
}

/**
 * \brief log a warning
 * 
 * \param message the warning message
 * \param ... variadic print arguments
 */
void DebugPort::warning(const char* message, ...) {
    va_list args;
    va_start(args, message);
    log_message(message, "WARNING", args);
    va_end(args);
}

/**
 * \brief log an error
 * 
 * \param message the error to log
 * \param ... variadic print arguments
 */
void DebugPort::error(const char* message, ...) {
    va_list args;
    va_start(args, message);
    log_message(message, "ERROR", args);
    va_end(args);
}
