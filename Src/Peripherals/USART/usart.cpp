/*! \file usart.c
*
*  \brief setup the usart peripherals in the application.
*
*
*  \author Graham Riches
*/


/********************************** Includes *******************************************/
#include "usart.h"
#include "hal_usart.h"
#include "hal_rcc.h"
#include "hal_gpio.h"
#include "core_cm4.h"
#include "ring_buffer.h"

/*********************************** Consts ********************************************/


/************************************ Types ********************************************/


/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/


/******************************** Local Variables **************************************/
static RingBuffer<uint8_t> usart_buffer(100);
static uint8_t buffer[10] = { 0 };
static uint8_t count = 0;

/****************************** Functions Prototype ************************************/
static void initialize_debug_usart( void );


/****************************** Functions Definition ***********************************/
/**
* \brief    Initialize the module variables and resources.
*
* \param    None
* \retval   None
*/
void USART_initialize( void )
{
   /* Initialize the module here*/
   initialize_debug_usart();
}

/**
 * \brief setup the debug port USART2
 * 
 */
static void initialize_debug_usart( void )
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

   /* configure the usart */
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
   
   /* enable the interrupts */
   NVIC_SetPriority( USART2_IRQn, 2 ); //!< TODO: set priority based on logical reasoning :D
   NVIC_EnableIRQ( USART2_IRQn );   

   /* enable the UART */
   USART::write_control_register( USART2, USART::ControlRegister1::usart_enable, 0x01 );
}

/**
 * \brief interrupt callback for the uart
 * \note these are defined in the vector table, and therefore need C linkage :(
 */
extern "C" {
void USART2_IRQHandler( void )
{   
   //usart_buffer.put( USART2->DR );
   buffer[count++] = USART2->DR & 0xFF;
}
}