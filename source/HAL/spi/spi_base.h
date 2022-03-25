/**
 * \file spi_base.h
 * \author Graham Riches (you@domain.com)
 * \brief 
 * \version 0.1
 * \date 2022-03-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once


/********************************** Includes *******************************************/
#include "spi_definitions.h"
#include "hal_gpio.h"
#include "hal_nvic.h"
#include "stm32f4xx.h"

namespace HAL::spi
{

/**
 * \brief Base class for SPI peripherals 
 */
class spi_base {
  public:
    spi_base(SPI_TypeDef* spi_peripheral_address, gpio::output_pin& chip_select);

    
    virtual void initialize() = 0;


    uint8_t read_status_register(status_register reg);


    void write_control_register(control_register_1 reg, uint8_t value);


    void write_control_register(control_register_2 reg, uint8_t value);


    uint8_t read_control_register(control_register_1 reg);


    uint8_t read_control_register(control_register_2 reg);


    void set_baudrate(baudrate_prescaler prescaler);

  protected:
    SPI_TypeDef* m_peripheral;
    gpio::output_pin* m_chip_select;    
};
};