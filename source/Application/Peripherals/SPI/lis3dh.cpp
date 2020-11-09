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
#include "hal_rcc.h"

/*********************************** Consts ********************************************/

/************************************ Types ********************************************/

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/
/* create the pins */
HAL::OutputPin accelerometer_chip_select( GPIOE, HAL::Pins::pin_3, HAL::PinMode::output, HAL::Speed::low, HAL::PullMode::pull_up, HAL::OutputMode::push_pull );
//HAL::AlternateModePin accelerometer_sck( GPIOA, HAL::Pins::pin_2, HAL::PinMode::alternate, HAL::Speed::very_high, HAL::PullMode::pull_up, HAL::OutputMode::push_pull, HAL::AlternateMode::af5 );
//HAL::AlternateModePin accelerometer_miso( GPIOA, HAL::Pins::pin_6, HAL::PinMode::alternate, HAL::Speed::very_high, HAL::PullMode::pull_up, HAL::OutputMode::push_pull, HAL::AlternateMode::af5 );
//HAL::AlternateModePin accelerometer_mosi( GPIOA, HAL::Pins::pin_7, HAL::PinMode::alternate, HAL::Speed::very_high, HAL::PullMode::pull_up, HAL::OutputMode::push_pull, HAL::AlternateMode::af5 );


LIS3DH accelerometer( SPI1, accelerometer_chip_select );

/******************************** Local Variables **************************************/

/****************************** Functions Prototype ************************************/

/****************************** Functions Definition ***********************************/
/**
 * \brief Construct a new LIS3DH::LIS3DH object
 * 
 * \param spi_peripheral_address the memory mapped peripheral address
 * \param chip_select the chip select pin
 */
LIS3DH::LIS3DH( SPI_TypeDef *spi_peripheral_address, HAL::OutputPin chip_select )
: HAL::SPIPolling( spi_peripheral_address, chip_select )
{

}


/**
 * \brief setup a LIS3DH accelerometer
 */
void LIS3DH::initialize( void )
{   
   using namespace HAL;

   /* enable the peripheral clock */
   reset_control_clock.set_apb2_clock( APB2Clocks::spi_1, true );

   /* setup the SPI control register settings */
   this->write_control_register( SPIControlRegister1::master_select, 0x01 );
   this->write_control_register( SPIControlRegister1::clock_polarity, 0x01 );
   this->write_control_register( SPIControlRegister1::clock_phase, 0x01 );
   this->set_baudrate( SPIBaudratePrescaler::prescaler_16 ); //!< 84MHz / 16 = 5.25 MHz

   this->write_control_register( SPIControlRegister1::spi_enable, 0x01 );
}