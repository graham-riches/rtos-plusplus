/*! \file hal_spi.h
*
*  \brief hal_spi module functions and variables declarations.
*
*
*  \author Graham Riches
*/

#ifndef __HAL_SPI_H__
#define __HAL_SPI_H__

/********************************** Includes *******************************************/
#include "stm32f4xx.h"
#include "hal_interrupt.h"
#include "hal_gpio.h"
#include "ring_buffer.h"


namespace HAL
{
/*********************************** Consts ********************************************/

/************************************ Types ********************************************/
/**
 * \brief bit offsets for SPI control register 1
 */
enum class SPIControlRegister1 : unsigned
{
   clock_phase = 0,
   clock_polarity = 1,
   master_select = 2,
   baudrate = 3,
   spi_enable = 6,
   lsb_first = 7,
   internal_slave_select = 8,
   software_slave_management = 9,
   receive_only = 10,
   data_frame_format = 11,
   crc_next = 12,
   crc_enable = 13,
   bidirectional_mode_select = 14,
   bidirectional_mode_enable = 15
};

/**
 * \brief bit offsets for SPI control register 2 
 */
enum class SPIControlRegister2 : unsigned
{
   receive_dma_enable = 0,
   transmit_dma_enable = 1,
   slave_select_output_enable = 2,
   frame_format = 4,
   error_interrupt_enable = 5,
   receive_interrupt_enable = 6,
   transmit_interrupt_enable = 7
};

/**
 * \brief enumeration of bit offsets for the SPI status register 
 */
enum class SPIStatusRegister : unsigned
{
   receive_data_available = 0,
   transmit_data_empty = 1,
   channel_side = 2,
   underrun_error = 3,
   crc_error = 4,
   mode_fault = 5,
   overrun_error = 6,
   busy = 7,
   frame_format_error = 8
};

/**
 * \brief enumeration of baudrate scalers for SPI 
 */
enum class SPIBaudratePrescaler : unsigned 
{
   prescaler_2 = 0b000,
   prescaler_4 = 0b001,
   prescaler_8 = 0b010,
   prescaler_16 = 0b011,
   prescaler_32 = 0b100,
   prescaler_64 = 0b101,
   prescaler_128 = 0b110,
   prescaler_256 = 0b111
};

/**
 * \brief base class for SPI peripherals 
 */
class SPIBase
{
   protected:
   SPI_TypeDef *peripheral;
   OutputPin chip_select;

   public:
   SPIBase( SPI_TypeDef *spi_peripheral_address, OutputPin chip_select );

   /* interface setup function */
   virtual void initialize( ){ };

   bool read_status_register( SPIStatusRegister reg );
   void write_control_register( SPIControlRegister1 reg, uint8_t value );
   void write_control_register( SPIControlRegister2 reg, uint8_t value );
   void set_baudrate( SPIBaudratePrescaler prescaler );
};

/**
 * \brief class to manage polling based SPI peripherals
 */
class SPIPolling : protected SPIBase
{
   private:
   void read_write( uint8_t *tx_buffer, uint8_t *rx_buffer, uint16_t size );

   public:
   SPIPolling( SPI_TypeDef *spi_peripheral_address, OutputPin chip_select )
   : SPIBase( spi_peripheral_address, chip_select )
   { }

   void read( uint8_t *rx_buffer, uint16_t size );
   void write( uint8_t *tx_buffer, uint16_t size );
};


/**
 * \brief class to manage interrupt driven SPI peripherals
 */
class SPIInterrupt : protected SPIBase, public HAL::InterruptPeripheral
{
   protected:
   RingBuffer<uint8_t> tx_buffer;
   RingBuffer<uint8_t> rx_buffer;

   public:
   SPIInterrupt( SPI_TypeDef *spi_peripheral_address, OutputPin chip_select, size_t tx_size, size_t rx_size )
   : SPIBase( spi_peripheral_address, chip_select )
   , tx_buffer(tx_size)
   , rx_buffer(rx_size)
   {}

   void irq_handler( uint8_t type );
   /* TODO: implement this properly once polling driven SPI is working */
   //void send( uint8_t *data, uint16_t size );
};


/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/

/****************************** Functions Prototype ************************************/

};     // namespace HAL
#endif /* __HAL_SPI_H__ */
