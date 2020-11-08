/*! \file hal_spi.c
*
*  \brief HAL SPI interface.
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "hal_spi.h"

namespace HAL
{

/*********************************** Consts ********************************************/

/************************************ Types ********************************************/

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/

/******************************** Local Variables **************************************/

/****************************** Functions Prototype ************************************/

/****************************** Functions Definition ***********************************/
/**
 * \brief Construct a new SPIBase::SPIBase object
 * 
 * \param spi_peripheral_address the address of the memory mapped peripheral
 */
SPIBase::SPIBase( SPI_TypeDef *spi_peripheral_address )
{
   this->peripheral = spi_peripheral_address;
}

/**
 * \brief read a status register flag from the peripheral
 * 
 * \param reg the register to read
 * \retval state of the register
 */
bool SPIBase::read_status_register( SPIStatusRegister reg )
{
   return static_cast<bool>( this->peripheral->SR & ( 0x01u << static_cast<uint8_t>( reg ) ) );
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
   uint8_t register_mask = ( reg != SPIControlRegister1::baudrate ) ? 0x001 : 0x111;

   /* clear the register */
   this->peripheral->CR1 &= ~static_cast<uint16_t>( ( static_cast<uint8_t>( 0x01 ) << static_cast<uint8_t>( reg ) ) );

   /* write the value */
   this->peripheral->CR1 |= static_cast<uint16_t>( ( ( value & 0x01 ) << static_cast<uint8_t>( reg ) ) );
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
 * \brief set the SPI baudrate (in CR1)
 * 
 * \param prescaler prescaler value
 */
void SPIBase::set_baudrate( SPIBaudratePrescaler prescaler )
{
   this->write_control_register( SPIControlRegister1::baudrate, static_cast<uint8_t>( prescaler ) );
}


/**
 * \brief interrupt service routing for interrupt drive SPI peripherals
 * 
 * \param type interrupt type (not used)
 */
void SPIInterrupt::irq_handler( uint8_t type )
{
   PARAMETER_NOT_USED( type );
   
   uint32_t control_reg1 = this->peripheral->CR1;

   /* handle any rx interrupts */
   bool rx_data_available = this->read_status_register( SPIStatusRegister::receive_data_available );
   bool rx_interrupt_enabled = static_cast<bool>( control_reg1 & ( 0x01 << static_cast<uint32_t>( SPIControlRegister2::receive_interrupt_enable ) ) );

   if ( ( rx_data_available ) && ( rx_interrupt_enabled ) )
   {
      uint8_t data = static_cast<uint8_t>( this->peripheral->DR & 0xFF );
      this->rx_buffer.put( data );
   }

   /* handle any tx interrupts */
   bool tx_data_empty = this->read_status_register( SPIStatusRegister::transmit_data_empty );
   bool tx_interrupt_enabled = static_cast<bool>( control_reg1 & ( 0x01 << static_cast<uint32_t>( SPIControlRegister2::transmit_interrupt_enable ) ) );

   if ( ( tx_data_empty ) && ( tx_interrupt_enabled ) )
   {
      uint8_t data = this->tx_buffer.get( );
      this->peripheral->DR = data;

      /* if there is no more data to send, disable the interrupt */
      if ( this->tx_buffer.is_empty( ) )
      {
         this->write_control_register( SPIControlRegister2::transmit_interrupt_enable, 0x00 );
      }
   }
}


};  // namespace HAL
