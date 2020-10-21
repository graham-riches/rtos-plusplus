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


/*********************************** Consts ********************************************/


/************************************ Types ********************************************/
/**
 * \brief enumeration of GPIO LED pins for the onboard LEDs
 */
typedef enum
{
    GPIO_LED_GREEN = 0,
    GPIO_LED_ORANGE,
    GPIO_LED_RED,
    GPIO_LED_BLUE,
    GPIO_TOTAL_LEDS
} GPIO_LED_t;

typedef enum
{
    GPIO_LED_OFF = 0,
    GPIO_LED_ON,
} GPIO_LED_state_t;


/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/


/****************************** Functions Prototype ************************************/
void GPIO_init( void );
void GPIO_setLED( GPIO_LED_t led, GPIO_LED_state_t state );



#endif /* __GPIO_H__ */
