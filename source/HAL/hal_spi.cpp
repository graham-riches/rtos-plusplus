/*! \file hal_spi.c
*
*  \brief HAL SPI interface.
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "hal_spi.h"
#include <cassert>

namespace HAL
{

/*********************************** Consts ********************************************/

/************************************ Types ********************************************/

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/

/******************************** Local Variables **************************************/

/****************************** Functions Prototype ************************************/

/****************************** SPIBase Definitions ***********************************/
/**
 * \brief Construct a new SPIBase::SPIBase object
 * 
 * \param spi_peripheral_address the address of the memory mapped peripheral
 * \param chip_select a GPIO output pin used as the chip select
 * \note the output pin object should be created before the SPI object
 */
SPIBase::SPIBase( SPI_TypeDef *spi_peripheral_address, OutputPin chip_select )
{
   assert( spi_peripheral_address != nullptr );
   this->peripheral = spi_peripheral_address;
   this->chip_select = chip_select;

   /* pull the chip select high by default */
   this->chip_select.set( true );
}

/**
 * \brief read a status register flag from the peripheral
 * 
 * \param reg the register to read
 * \retval state of the register
 */
uint8_t SPIBase::read_status_register( SPIStatusRegister reg )
{
   return static_cast<uint8_t>( this->peripheral->SR & ( 0x01u << static_cast<uint8_t>( reg ) ) );
}

/**
 * \brief write a register in the SPI CR1
 * 
 * \param reg the selected register
 * \param value value to write
 */
void SPIBase::write_control_register( SPIControlRegister1 reg, uint8_t value )
{
   /* clear a single bit unless the register is the baudrate (3 bits ) */
   uint8_t register_mask = ( reg != SPIControlRegister1::baudrate ) ? 0b1 : 0b111;

   /* clear the register */
   this->peripheral->CR1 &= ~static_cast<uint16_t>( ( register_mask << static_cast<uint8_t>( reg ) ) );

   /* write the value */
   this->peripheral->CR1 |= static_cast<uint16_t>( ( ( value & register_mask ) << static_cast<uint8_t>( reg ) ) );
}

/**
 * \brief write a register in the SPI CR2
 * 
 * \param reg the selected register
 * \param value the value to write
 */
void SPIBase::write_control_register( SPIControlRegister2 reg, uint8_t value )
{
   /* clear the register */
   this->peripheral->CR2 &= ~static_cast<uint16_t>( ( static_cast<uint8_t>( 0x01 ) << static_cast<uint8_t>( reg ) ) );

   /* write the value */
   this->peripheral->CR2 |= static_cast<uint16_t>( ( ( value & 0x01 ) << static_cast<uint8_t>( reg ) ) );
}

/**
 * \brief read the state of a control register in CR1
 * 
 * \param reg the register to read
 * \retval uint8_t register value
 */
uint8_t SPIBase::read_control_register( SPIControlRegister1 reg )
{
   uint8_t register_mask = ( reg != SPIControlRegister1::baudrate ) ? 0b1 : 0b111;
   uint8_t register_value = static_cast<uint8_t>( this->peripheral->CR1 & ( register_mask << static_cast<uint8_t>( reg ) ) );
   return register_value;
}

/**
 * \brief read the state of a register in CR2
 * 
 * \param reg the register to read
 * \retval uint8_t register value
 */
uint8_t SPIBase::read_control_register( SPIControlRegister2 reg )
{
   uint8_t register_mask = 0b1;
   uint8_t register_value = static_cast<uint8_t>( this->peripheral->CR2 & ( register_mask << static_cast<uint8_t>( reg ) ) );
   return register_value;
}

/**
 * \brief set the SPI baudrate (in CR1)
 * 
 * \param prescaler prescaler value
 */
void SPIBase::set_baudrate( SPIBaudratePrescaler prescaler )
{
   this->write_control_register( SPIControlRegister1::baudrate, static_cast<uint8_t>( prescaler ) );
}

/****************************** SPIPolling Definitions ***********************************/
/**
 * \brief private function to read/write spi data in a polling/blocking loop
 * 
 * \param tx_buffer pointer to the tx_buffer
 * \param rx_buffer pointer to the rx_buffer
 * \param size size of the message
 */
void SPIPolling::read_write( uint8_t *tx_buffer, uint8_t *rx_buffer, uint16_t size )
{
   /* pull the chip select low */
   this->chip_select.set( false );

   /* send/receive all of the data */
   while ( size > 0 )
   {
      /* wait for the transmit buffer to clear */
      while ( this->read_status_register( SPIStatusRegister::transmit_data_empty ) == false )
      { }
      
      /* put data outgoing into the data register if sending, or junk if receiving */
      this->peripheral->DR = ( tx_buffer != nullptr ) ? *tx_buffer++ : static_cast<uint8_t>( 0x00 );

      /* wait for the receive buffer to clear */
      while ( this->read_status_register( SPIStatusRegister::receive_data_available ) == false )
      { }

      uint8_t rx_data = static_cast<uint8_t>( this->peripheral->DR );

      /* receive data */
      if ( rx_buffer != nullptr )
      {
         *rx_buffer++ = rx_data;
      }

      size--;
   }

   /* release the chip select */
   this->chip_select.set( true );
}

/**
 * \brief function to receive data on a SPI polling peripheral
 * 
 * \param rx_buffer buffer to store the data in
 * \param size how much data to read
 */
void SPIPolling::read( uint8_t *rx_buffer, uint16_t size )
{
   this->read_write( nullptr, rx_buffer, size );
}

/**
 * \brief write data on a polling SPI peripheral
 * 
 * \param tx_buffer buffer to send
 * \param size size of the buffer
 */
void SPIPolling::write( uint8_t *tx_buffer, uint16_t size )
{
   this->read_write( tx_buffer, nullptr, size );
}

/****************************** SPIInterrupt Definitions ***********************************/
/**
 * \brief interrupt service routing for interrupt drive SPI peripherals
 * 
 * \param type interrupt type (not used)
 */
void SPIInterrupt::irq_handler( uint8_t type )
{
   PARAMETER_NOT_USED( type );
   /* enable the chip select */
   this->chip_select.set( false );

   while ( !this->tx_buffer.is_empty() )
   {
      /* wait for the transmit buffer to clear */
      while ( this->read_status_register( SPIStatusRegister::transmit_data_empty ) == false )
      { }
      
      /* put data outgoing into the data register */      
      this->peripheral->DR = this->tx_buffer.get();

      /* wait for the receive buffer to clear */
      while ( this->read_status_register( SPIStatusRegister::receive_data_available ) == false )
      { }

      /* receive data into the rx buffer */
      this->rx_buffer.put( static_cast<uint8_t>( this->peripheral->DR ) );      
   }

   /* disable the interrupt */
   this->write_control_register( SPIControlRegister2::transmit_interrupt_enable, 0x00 );

   /* disable the chip select */
   this->chip_select.set( true );
}

/**
 * \brief send data on an interrupt driven SPI
 * 
 */
void SPIInterrupt::send( uint8_t *data, uint16_t size )
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
   this->write_control_register( SPIControlRegister2::transmit_interrupt_enable, 0x01 );
}


};  // namespace HAL
