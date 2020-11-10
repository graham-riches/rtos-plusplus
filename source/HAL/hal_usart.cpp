/*! \file hal_usart.c
*
*  \brief HAL++ implementation of the STM32 USART peripherals.
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "hal_usart.h"
#include <string.h>
#include <cassert>

namespace HAL
{

/*********************************** Consts ********************************************/

/************************************ Types ********************************************/

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/

/******************************** Local Variables **************************************/

/****************************** Functions Prototype ************************************/

/****************************** Function Definitions ***********************************/
/**
 * \brief Construct a new USARTBase::USARTBase object
 * 
 * \param usart the usart peripheral address
 */
USARTBase::USARTBase( USART_TypeDef *usart )
{
   assert( usart != nullptr );
   this->peripheral = usart;
}

/**
 * \brief read a value from the status register
 * 
 * \param reg the register 
 * \retval register value
 */
uint8_t USARTBase::read_status_register( USARTStatusRegister reg )
{
   return static_cast<uint8_t>( this->peripheral->SR & ( 0x01u << static_cast<uint8_t>( reg ) ) );
}

/**
 * \brief write a value into a usart peripherals CR1
 * 
 * \param reg the register
 * \param value value register value to set
 */
void USARTBase::write_control_register( USARTControlRegister1 reg, uint8_t value )
{
   /* clear the register */
   this->peripheral->CR1 &= ~static_cast<uint16_t>( ( static_cast<uint8_t>( 0x01 ) << static_cast<uint8_t>( reg ) ) );

   /* write the value */
   this->peripheral->CR1 |= static_cast<uint16_t>( ( ( value & 0x01 ) << static_cast<uint8_t>( reg ) ) );
}

/**
 * \brief write a value into a usart peripherals CR2
 * 
 * \param reg the register
 * \param value value
 */
void USARTBase::write_control_register( USARTControlRegister2 reg, uint8_t value )
{
   /* clear the register */
   this->peripheral->CR2 &= ~static_cast<uint16_t>( ( static_cast<uint8_t>( 0x01 ) << static_cast<uint8_t>( reg ) ) );

   /* write the value */
   this->peripheral->CR2 |= static_cast<uint16_t>( ( ( value & 0x01 ) << static_cast<uint8_t>( reg ) ) );
}

/**
 * \brief write a value into a usart peripherals CR3
 * 
 * \param reg the register
 * \param value value
 */
void USARTBase::write_control_register( USARTControlRegister3 reg, uint8_t value )
{
   /* clear the register */
   this->peripheral->CR3 &= ~static_cast<uint16_t>( ( static_cast<uint8_t>( 0x01 ) << static_cast<uint8_t>( reg ) ) );

   /* write the value */
   this->peripheral->CR3 |= static_cast<uint16_t>( ( ( value & 0x01 ) << static_cast<uint8_t>( reg ) ) );
}

/**
 * \brief read a control register from CR1
 * 
 * \param reg register to read
 * \retval uint8_t value of the register
 */
uint8_t USARTBase::read_control_register( USARTControlRegister1 reg )
{
   uint8_t register_mask = 0b1;
   uint8_t register_value = static_cast<uint8_t>( this->peripheral->CR1 & ( register_mask << static_cast<uint8_t>( reg ) ) );
   return register_value;
}

/**
 * \brief read a value from CR2
 * 
 * \param reg register to read
 * \retval uint8_t value of the register
 */
uint8_t USARTBase::read_control_register( USARTControlRegister2 reg )
{
   uint8_t register_mask = 0b1;
   uint8_t register_value = static_cast<uint8_t>( this->peripheral->CR2 & ( register_mask << static_cast<uint8_t>( reg ) ) );
   return register_value;
}

/**
 * \brief read a value from CR3
 * 
 * \param reg register to read
 * \retval uint8_t value of the register
 */
uint8_t USARTBase::read_control_register( USARTControlRegister3 reg )
{
   uint8_t register_mask = 0b1;
   uint8_t register_value = static_cast<uint8_t>( this->peripheral->CR3 & ( register_mask << static_cast<uint8_t>( reg ) ) );
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
void USARTBase::set_baudrate( HAL::Clocks clock, uint32_t baudrate )
{
   uint32_t base_clock_rate = HAL::reset_control_clock.get_clock_speed( clock );

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
   this->peripheral->BRR = static_cast<uint16_t>( register_value );
}

/**
 * \brief handle an interrupt on a usart peripheral
 * \param type type of the interrupt
 */
void USARTInterrupt::irq_handler( uint8_t type )
{
   PARAMETER_NOT_USED( type );

   /* handle any rx interrupts */
   bool rx_data_available = this->read_status_register( USARTStatusRegister::receive_data_available );
   bool rx_interrupt_enabled = this->read_control_register( USARTControlRegister1::receive_interrupt_enable );

   if ( ( rx_data_available ) && ( rx_interrupt_enabled ) )
   {
      uint8_t data = static_cast<uint8_t>( this->peripheral->DR & 0xFF );
      this->rx_buffer.put( data );
   }

   /* handle any tx interrupts */
   bool tx_data_empty = this->read_status_register( USARTStatusRegister::transmit_data_empty );
   bool tx_interrupt_enabled = this->read_control_register( USARTControlRegister1::transmit_interrupt_enable );

   if ( ( tx_data_empty ) && ( tx_interrupt_enabled ) )
   {
      uint8_t data = this->tx_buffer.get( );
      this->peripheral->DR = data;

      /* if there is no more data to send, disable the interrupt */
      if ( this->tx_buffer.is_empty( ) )
      {
         this->write_control_register( USARTControlRegister1::transmit_interrupt_enable, 0x00 );
      }
   }
}

/**
 * \brief send data over a interrupt based usart
 * 
 * \param data pointer to the data to send
 * \param size amount of data
 */
void USARTInterrupt::send( uint8_t *data, uint16_t size )
{
   /* put the data on the buffer */
   while ( size-- )
   {
      this->tx_buffer.put( *data++ );
      if ( this->tx_buffer.is_full( ) )
      {
         break;
      }
   }

   /* enable the tx interrupt */
   this->write_control_register( USARTControlRegister1::transmit_interrupt_enable, 0x01 );
}

/**
 * \brief overloaded send for string based messages
 * 
 * \param data string data
 */
void USARTInterrupt::send( const char *data )
{
   //!< TODO: possible unsafe strlen here?
   uint16_t size = strlen( data );

   for ( uint16_t i = 0; i < size; i++ )
   {
      uint8_t dataByte = (uint8_t)data[i];
      this->tx_buffer.put( dataByte );

      if ( this->tx_buffer.is_full( ) )
      {
         break;
      }
   }

   /* enable the tx interrupt */
   this->write_control_register( USARTControlRegister1::transmit_interrupt_enable, 0x01 );
}

/**
 * \brief overloaded send for a single character
 * 
 * \param data the character to send
 */
void USARTInterrupt::send( const char data )
{
   this->tx_buffer.put( static_cast<uint8_t>( data ) );
   
   /* enable the tx interrupt */
   this->write_control_register( USARTControlRegister1::transmit_interrupt_enable, 0x01 );
}

};  // namespace HAL
