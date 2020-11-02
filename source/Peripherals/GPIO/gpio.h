/*! \file gpio.h
*
*  \brief gpio module functions and variables declarations.
*
*
*  \author Graham Riches
*/

#ifndef __GPIO_H__
#define __GPIO_H__


/********************************** Includes *******************************************/
#include "common.h"
#include "hal_gpio.h"

/*********************************** Consts ********************************************/


/************************************ Types ********************************************/


/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/
/* declare the LED pins */
extern HAL::GPIO::OutputPin green_led;
extern HAL::GPIO::OutputPin orange_led;
extern HAL::GPIO::OutputPin red_led;
extern HAL::GPIO::OutputPin blue_led;

/****************************** Functions Prototype ************************************/
void GPIO_initialize( void );


#endif /* __GPIO_H__ */
