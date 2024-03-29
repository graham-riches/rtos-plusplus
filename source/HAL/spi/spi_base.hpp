// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Graham Riches

#pragma once


/********************************** Includes *******************************************/
#include "spi_definitions.hpp"
#include "hal_gpio.hpp"
#include "hal_nvic.hpp"
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