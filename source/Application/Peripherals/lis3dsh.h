/*! \file lis3dsh.h
*
*  \brief accelerometer module functions and variables declarations.
*
*
*  \author Graham Riches
*/

#pragma once

/********************************** Includes *******************************************/
#include "common.h"
#include "hal_gpio.h"
#include "hal_spi.h"
#include "ring_buffer.h"
#include "stm32f4xx.h"


/*********************************** Consts ********************************************/

/************************************ Types ********************************************/
/**
 * \brief enumeration of register addresses
 * \note currently only contains select registers
 */
enum class LIS3DSHRegisters : unsigned {
    who_am_i = 0x0f,
    control_register_1 = 0x21,
    control_register_2 = 0x22,
    control_register_3 = 0x23,
    control_register_4 = 0x20,
    control_register_5 = 0x24,
    control_register_6 = 0x25,
    status_register = 0x27,
    output_x = 0x28,
    output_y = 0x2a,
    output_z = 0x2c,
};

/**
 * \brief enumeration of output sample rates
 * \note this is directly mapped to control register 4 and automatically
 *       enables all axes as well (XYZ)
 */
enum class LIS3DSHDataRate : unsigned {
    power_down = 0b00000000,
    sample_3Hz = 0b00010111,
    sample_6Hz = 0b00100111,
    sample_12Hz = 0b00110111,
    sample_25Hz = 0b01000111,
    sample_50Hz = 0b01010111,
    sample_100Hz = 0b01100111,
    sample_400Hz = 0b01110111,
    sample_800Hz = 0b10000111,
    sample_1600Hz = 0b10010111
};

/**
 * \brief enumeration to set the resolution of the accelerometer
 * \note this maps to control register 5 and accordingly sets some
 *       filter parameters (default to 800Hz Anti-alias filter)
 */
enum class LIS3DSHResolution : unsigned {
    resolution_2g = 0b00000000,
    resolution_4g = 0b00001000,
    resolution_6g = 0b00010000,
    resolution_8g = 0b00011000,
    resolution_16g = 0b00100000,
};

/**
 * \brief class for the LIS302DL accelerometer
 * \todo Rework buffer sizes after fixing up interrupts
 */
class LIS3DSH {
  private:    
    uint16_t conversion_factor;
    ring_buffer<float, 16> x_data;
    ring_buffer<float, 16> y_data;
    ring_buffer<float, 16> z_data;
    
    uint8_t read_register(LIS3DSHRegisters reg);
    void write_register(LIS3DSHRegisters reg, uint8_t value);
    void spi_irq_handler(void);
    void exti_0_irq_handler(void);

  public:
    LIS3DSH(SPI_TypeDef* spi_peripheral_address, HAL::OutputPin chip_select);
    void initialize(void);
    uint8_t self_test(void);
    void set_data_rate(LIS3DSHDataRate rate);
    void set_resolution(LIS3DSHResolution resolution);
    void irq_handler(uint8_t type);
};

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/
extern LIS3DSH accelerometer;
