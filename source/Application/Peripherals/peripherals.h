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

/*********************************** Consts ********************************************/

/************************************ Types ********************************************/

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/
/* LEDS */
extern HAL::OutputPin green_led;
extern HAL::OutputPin orange_led;
extern HAL::OutputPin red_led;
extern HAL::OutputPin blue_led;

/****************************** Functions Prototype ************************************/
void initialize_peripherals(void);
