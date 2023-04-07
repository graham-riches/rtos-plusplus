// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Graham Riches

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
