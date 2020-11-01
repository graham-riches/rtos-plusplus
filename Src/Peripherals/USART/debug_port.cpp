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

/*********************************** Consts ********************************************/
constexpr uint8_t debug_port_buffer_size = 128;

/************************************ Types ********************************************/

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/
DebugPort debug_port( USART2, debug_port_buffer_size, debug_port_buffer_size );

/******************************** Local Variables **************************************/

/****************************** Functions Prototype ************************************/

/****************************** Functions Definition ***********************************/
/**
 * \brief initialize the debug port with the correct HW settings
 */
void DebugPort::initialize( void )
{
   using namespace HAL;

   /* enable the GPIO clocks and the USART clocks */
   ResetControlClock::set_apb1_clock( ResetControlClock::APB1Clocks::usart_2, true );
   ResetControlClock::set_ahb1_clock( ResetControlClock::AHB1Clocks::gpio_a, true );

   /* setup the GPIO pins with the appropriate AF modes and outputs: see data sheet for pin AF mapping */
   GPIO::set_alternate_mode( GPIOA, GPIO::Pins::pin_2, GPIO::AlternateMode::af7 );
   GPIO::set_alternate_mode( GPIOA, GPIO::Pins::pin_3, GPIO::AlternateMode::af7 );
   GPIO::initialize_pin( GPIOA, GPIO::Pins::pin_2, GPIO::PinMode::alternate, GPIO::Speed::very_high, GPIO::PullMode::pull_up, GPIO::OutputMode::push_pull );
   GPIO::initialize_pin( GPIOA, GPIO::Pins::pin_3, GPIO::PinMode::alternate, GPIO::Speed::very_high, GPIO::PullMode::pull_up, GPIO::OutputMode::push_pull );

   /* configure the usart with the application specific settings */
   USART::write_control_register( USART2, USART::ControlRegister1::parity_selection, 0x00 );
   USART::write_control_register( USART2, USART::ControlRegister1::word_length, 0x00 );
   USART::write_control_register( USART2, USART::ControlRegister2::stop_bits, 0x00 );
   USART::write_control_register( USART2, USART::ControlRegister3::cts_enable, 0x00 );
   USART::write_control_register( USART2, USART::ControlRegister3::rts_enable, 0x00 );
   USART::set_baudrate( USART2, ResetControlClock::Clocks::APB1, 115200 );

   /* enable the usart and interrupts */
   USART::write_control_register( USART2, USART::ControlRegister1::receiver_enable, 0x01 );
   USART::write_control_register( USART2, USART::ControlRegister1::transmitter_enable, 0x01 );
   USART::write_control_register( USART2, USART::ControlRegister1::receive_interrupt_enable, 0x01 );

   /* register the interrupt in the hal interrupts table */
   Interrupt::register_callback( Interrupt::InterruptName::usart_2, this, 0, 2 );

   /* enable the UART */
   USART::write_control_register( USART2, USART::ControlRegister1::usart_enable, 0x01 );
}

/**
 * \brief log a debug message
 * 
 * \param message the message to send
 */
void DebugPort::debug( const char *message )
{
   this->send( "DEBUG: " );
   this->send( message );
   this->send( "\r\n" );
}

/**
 * \brief log an info message
 * 
 * \param message the message to send
 */
void DebugPort::info( const char *message )
{
   this->send( "INFO: " );
   this->send( message );
   this->send( "\r\n" );
}

/**
 * \brief log a warning
 * 
 * \param message the warning message
 */
void DebugPort::warning( const char *message )
{
   this->send( "WARNING: " );
   this->send( message );
   this->send( "\r\n" );
}

/**
 * \brief log an error
 * 
 * \param message the error to log
 */
void DebugPort::error( const char *message )
{
   this->send( "ERROR: " );
   this->send( message );
   this->send( "\r\n" );
}
