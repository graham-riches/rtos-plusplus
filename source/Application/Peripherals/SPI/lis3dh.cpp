/*! \file accelerometer.c
*
*  \brief LIS302 Accelerometer peripheral.
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "lis3dh.h"
#include "board.h"
#include "hal_interrupt.h"
#include "hal_rcc.h"


/*********************************** Consts ********************************************/
constexpr uint8_t buffer_size = 128;
constexpr uint8_t device_read = 0x80;
constexpr uint8_t device_write = 0x00;

/************************************ Types ********************************************/

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/
/* create the pins */
static HAL::OutputPin
   accelerometer_chip_select( GPIOE, HAL::Pins::pin_3, HAL::PinMode::output, HAL::Speed::low, HAL::PullMode::pull_up, HAL::OutputMode::push_pull );
static HAL::AlternateModePin accelerometer_sck(
   GPIOA, HAL::Pins::pin_5, HAL::PinMode::alternate, HAL::Speed::very_high, HAL::PullMode::pull_up, HAL::OutputMode::push_pull, HAL::AlternateMode::af5 );
static HAL::AlternateModePin accelerometer_miso(
   GPIOA, HAL::Pins::pin_6, HAL::PinMode::alternate, HAL::Speed::very_high, HAL::PullMode::pull_up, HAL::OutputMode::push_pull, HAL::AlternateMode::af5 );
static HAL::AlternateModePin accelerometer_mosi(
   GPIOA, HAL::Pins::pin_7, HAL::PinMode::alternate, HAL::Speed::very_high, HAL::PullMode::pull_up, HAL::OutputMode::push_pull, HAL::AlternateMode::af5 );

LIS3DH accelerometer( SPI1, accelerometer_chip_select, buffer_size, buffer_size );

/******************************** Local Variables **************************************/

/****************************** Functions Prototype ************************************/

/****************************** Functions Definition ***********************************/
/**
 * \brief Construct a new LIS3DH::LIS3DH object
 * 
 * \param spi_peripheral_address the memory mapped peripheral address
 * \param chip_select the chip select pin
 * \param tx_size size of the tx buffer
 * \param rx_size size of the rx_buffer
 */
LIS3DH::LIS3DH( SPI_TypeDef *spi_peripheral_address, HAL::OutputPin chip_select, size_t tx_size, size_t rx_size )
   : HAL::SPIInterrupt( spi_peripheral_address, chip_select, tx_size, rx_size )
{ }

/**
 * \brief read the value of a register
 * 
 * \param reg the register to read
 * \retval uint8_t value of the register
 */
uint8_t LIS3DH::read_register_8( LIS3DHRegisters reg )
{
   uint16_t read_command =  static_cast<uint8_t>( reg ) | device_read;
   this->send( reinterpret_cast<uint8_t *>( &read_command ), sizeof( read_command ) );

   /* block until the register result returns */
   uint8_t data;
   while ( !this->rx_buffer.is_empty( ) )
   {
      data = this->rx_buffer.get( );
   }

   return data;
}

/**
 * \brief setup a LIS3DH accelerometer
 */
void LIS3DH::initialize( void )
{
   using namespace HAL;

   /* enable the peripheral clock */
   reset_control_clock.set_apb_clock( APB2Clocks::spi_1, true );

   /* setup the SPI control register settings */
   this->write_control_register( SPIControlRegister1::master_select, 0x01 );
   this->write_control_register( SPIControlRegister1::clock_polarity, 0x01 );
   this->write_control_register( SPIControlRegister1::clock_phase, 0x01 );
   this->write_control_register( SPIControlRegister2::slave_select_output_enable, 0x01 );
   this->set_baudrate( SPIBaudratePrescaler::prescaler_16 );  //!< 84MHz / 16 = 5.25 MHz

   /* register the interrupt */
   interrupt_manager.register_callback( InterruptName::spi_1, this, 0, 1 );

   /* enable the interrupt */
   this->write_control_register( SPIControlRegister2::receive_interrupt_enable, 0x01 );

   /* enable the peripheral */
   this->write_control_register( SPIControlRegister1::spi_enable, 0x01 );
}

/**
 * \brief test function for the accelerometer
 * \retval return the result of the who am I register
 */
uint8_t LIS3DH::self_test( void )
{
   uint8_t self_test = this->read_register_8( LIS3DHRegisters::who_am_i );
   return self_test;
}