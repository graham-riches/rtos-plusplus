/*! \file hal_usart.c
*
*  \brief HAL++ implementation of the STM32 USART peripherals.
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "hal_usart.h"

namespace HAL
{
namespace USART
{

/*********************************** Consts ********************************************/

/************************************ Types ********************************************/

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/

/******************************** Local Variables **************************************/

/****************************** Functions Prototype ************************************/

/****************************** Function Definitions ***********************************/
/**
 * \brief read a value from the status register
 * 
 * \param usart which usart peripheral to read
 * \param reg the register 
 * \retval register value
 */
bool read_status_register( USART_TypeDef *usart, StatusRegister reg )
{
   return static_cast<bool>( usart->SR & ( 0x01u << static_cast<uint8_t>( reg ) ) );
}

/**
 * \brief write a value into a usart peripherals CR1
 * 
 * \param usart the usart to write to
 * \param reg the register
 * \param value value
 */
void write_control_register( USART_TypeDef *usart, ControlRegister1 reg, uint8_t value )
{
   /* clear the register */
   usart->CR1 &= ~static_cast<uint16_t>( ( static_cast<uint8_t>( 0x01 ) << static_cast<uint8_t>( reg ) ) );

   /* write the value */
   usart->CR1 |= static_cast<uint16_t>( ( ( value & 0x01 ) << static_cast<uint8_t>( reg ) ) );
}

/**
 * \brief write a value into a usart peripherals CR2
 * 
 * \param usart the usart to write to
 * \param reg the register
 * \param value value
 */
void write_control_register( USART_TypeDef *usart, ControlRegister2 reg, uint8_t value )
{
   /* clear the register */
   usart->CR2 &= ~static_cast<uint16_t>( ( static_cast<uint8_t>( 0x01 ) << static_cast<uint8_t>( reg ) ) );

   /* write the value */
   usart->CR2 |= static_cast<uint16_t>( ( ( value & 0x01 ) << static_cast<uint8_t>( reg ) ) );
}

/**
 * \brief write a value into a usart peripherals CR3
 * 
 * \param usart the usart to write to
 * \param reg the register
 * \param value value
 */
void write_control_register( USART_TypeDef *usart, ControlRegister3 reg, uint8_t value )
{
   /* clear the register */
   usart->CR3 &= ~static_cast<uint16_t>( ( static_cast<uint8_t>( 0x01 ) << static_cast<uint8_t>( reg ) ) );

   /* write the value */
   usart->CR3 |= static_cast<uint16_t>( ( ( value & 0x01 ) << static_cast<uint8_t>( reg ) ) );
}

/**
 * \brief Set the baudrate for a usart peripheral
 * 
 * \param usart the usart peripheral
 * \param clock enum value of which peripheral clock the usart is associated with
 * \param baudrate baudrate to set
 * \todo remove base clock rate and get that direct from HAL_rcc
 * \todo account for oversampling mode
 */
void set_baudrate( USART_TypeDef *usart, HAL::ResetControlClock::Clocks clock, uint32_t baudrate )
{
   uint32_t base_clock_rate = HAL::ResetControlClock::get_clock_speed( clock );

   uint32_t usart_div = ( 100 * base_clock_rate ) / ( 16 * baudrate );

   /* calculate the integer component */
   uint32_t integer_component = usart_div / 100;

   /* calculate the fractional component */
   uint32_t fractional_temp = usart_div - ( 100 * integer_component );

   /* round the fractional component */
   uint32_t fractional_component = ( ( ( fractional_temp * 16 ) + 50 ) / 100 );

   /* build out the register value */
   uint32_t register_value = ( integer_component << 4 ) | ( fractional_component & 0x0F );

   /* write it to the register */
   usart->BRR = static_cast<uint16_t>( register_value );
}

/**
 * \brief handle an interrupt on a usart peripheral
 * \param type type of the interrupt
 */
void USARTInterrupt::irq_handler( uint8_t type )
{
   PARAMETER_NOT_USED( type );

   uint32_t status = this->peripheral->SR;
   uint32_t control_reg1 = this->peripheral->CR1;

   /* handle any rx interrupts */
   bool rx_data_available = static_cast<bool>( status & static_cast<uint32_t>( StatusRegister::receive_data_available ) );
   bool rx_interrupt_enabled = static_cast<bool>( control_reg1 & static_cast<uint32_t>( ControlRegister1::receive_interrupt_enable ) );

   if ( ( rx_data_available ) && ( rx_interrupt_enabled ) )
   {
      uint8_t data = static_cast<uint8_t>( this->peripheral->DR & 0xFF );
      this->rx_buffer.put( data );
   }

   /* handle any tx interrupts */
   bool tx_data_empty = static_cast<bool>( status & static_cast<uint32_t>( StatusRegister::transmit_data_empty ) );
   bool tx_interrupt_enabled = static_cast<bool>( control_reg1 & static_cast<uint32_t>( ControlRegister1::transmit_interrupt_enable ) );

   if ( ( tx_data_empty ) && ( tx_interrupt_enabled ) )
   {
      uint8_t data = this->tx_buffer.get( );
      this->peripheral->DR = data;

      /* if there is no more data to send, disable the interrupt */
      if ( this->tx_buffer.is_empty( ) )
      {
         write_control_register( this->peripheral, ControlRegister1::transmit_interrupt_enable, 0x00 );
      }
   }
}

};  // namespace USART
};  // namespace HAL
