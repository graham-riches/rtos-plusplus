/*! \file lis3dh.h
*
*  \brief accelerometer module functions and variables declarations.
*
*
*  \author Graham Riches
*/

#ifndef __LIS3DH_H__
#define __LIS3DH_H__


/********************************** Includes *******************************************/
#include "common.h"
#include "hal_spi.h"
#include "hal_gpio.h"
#include "stm32f4xx.h"


/*********************************** Consts ********************************************/


/************************************ Types ********************************************/
/**
 * \brief enumeration of register addresses
 * 
 */
enum class LIS3DHRegisters : unsigned
{
   auxilliary_status = 0x07,
   adc_1 = 0x08,
   adc_2 = 0x0a,
   adc_3 = 0x0c,
   who_am_i = 0x0f,
   control_register_0 = 0x1e,
   temperature_config = 0x1f,
   control_register_1 = 0x20,
   control_register_2 = 0x21,
   control_register_3 = 0x22,
   control_register_4 = 0x23,
   control_register_5 = 0x24,
   control_register_6 = 0x25,
   reference = 0x26,
   status_register = 0x27,
   output_x = 0x28,
   output_y = 0x2a,
   output_z = 0x2c,
   fifo_control = 0x2e,
   fifo_source = 0x2f,
   interrupt_1_config = 0x30,
   interrupt_1_threshold = 0x32,
   interrupt_1_duration = 0x33,
   interrupt_2_config = 0x34,
   interrupt_2_source = 0x35,
   interrupt_2_threshold = 0x36,
   interrupt_2_duration = 0x37,
   click_config = 0x38,
   click_source = 0x39,
   click_threshold = 0x3a,
   click_time_limit = 0x3b,
   time_latency = 0x3c,
   time_window = 0x3d,
   activate_threshold = 0x3e,
   activate_duration = 0x3f
};




/**
 * \brief class for the LIS3DH accelerometer
 */
class LIS3DH : public HAL::SPIInterrupt
{
   private:
   uint8_t read_register_8( LIS3DHRegisters register );

   public:
   LIS3DH( SPI_TypeDef *spi_peripheral_address, HAL::OutputPin chip_select, size_t tx_size, size_t rx_size );
   void initialize( void );
   uint8_t self_test( void );
};


/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/
extern LIS3DH accelerometer;

/****************************** Functions Prototype ************************************/



#endif /* __LIS3DH_H__ */
