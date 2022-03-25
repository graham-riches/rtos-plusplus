/**
 * \file peripherals.hpp
 * \author Graham Riches
 * \brief Setup any peripheral devices
 * \version 0.1
 * \date 2022-03-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

/********************************** Includes *******************************************/
#include "common.hpp"
#include "hal_gpio.hpp"

/******************************* Global Variables **************************************/
extern HAL::gpio::output_pin green_led;
extern HAL::gpio::output_pin orange_led;
extern HAL::gpio::output_pin red_led;
extern HAL::gpio::output_pin blue_led;

/****************************** Function Definitions ************************************/
/**
 * \brief Initialize peripheral devices
 */
void initialize_peripherals();
