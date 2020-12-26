/*! \file peripherals.h
*
*  \brief hal module functions and variables declarations.
*
*
*  \author Graham Riches
*/

#ifndef __PERIPHERALS_H__
#define __PERIPHERALS_H__

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

#endif /* __PERIPHERALS_H__ */
