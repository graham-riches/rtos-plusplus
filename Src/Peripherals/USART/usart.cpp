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

/*********************************** Consts ********************************************/


/************************************ Types ********************************************/


/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/


/******************************** Local Variables **************************************/


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

   /* enable the usart */
   USART::write_control_register( USART2, USART::ControlRegister1::receiver_enable, 0x01 );
   USART::write_control_register( USART2, USART::ControlRegister1::transmitter_enable, 0x01 );
   USART::write_control_register( USART2, USART::ControlRegister1::receive_interrupt_enable, 0x01 );
   

   /* enable the interrupts */

}