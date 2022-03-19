/*! \file peripherals.h
*
*  \brief hal module functions and variables declarations.
*
*
*  \author Graham Riches
*/

#pragma once

/********************************** Includes *******************************************/
#include "common.h"
#include "hal_gpio.h"

/******************************* Global Variables **************************************/
extern HAL::gpio::output_pin green_led;
extern HAL::gpio::output_pin orange_led;
extern HAL::gpio::output_pin red_led;
extern HAL::gpio::output_pin blue_led;

/****************************** Functions Prototype ************************************/
void initialize_peripherals(void);
