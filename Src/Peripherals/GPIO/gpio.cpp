/*! \file gpio.c
*
*  \brief HAL GPIO drivers.
*
*
*  \author Graham Riches
*/


/********************************** Includes *******************************************/
#include "gpio.h"
#include "stm32f4xx_gpio.h"
#include "board.h"
#include "hal_gpio.h"

/*********************************** Consts ********************************************/
#define GPIO_LED_BANK GPIOD


/************************************ Types ********************************************/


/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/


/******************************** Local Variables **************************************/

/****************************** Functions Prototype ************************************/


/****************************** Functions Definition ***********************************/
/**
* \name     GPIO_init
* \brief    Initialize the module variables and resources.
*
* \param    None
* \retval   None
*/
void GPIO_init( void )
{
   /* GPIO Ports Clock Enable */
   /* TODO: remove once the HAL RCC code is built out */
   RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD, ENABLE );


   /* configure the pins */
   using namespace HAL::GPIO;
   static OutputPin green_led( GPIO_LED_BANK, Pins::pin_12, Speed::low, PullMode::pull_down, OutputMode::push_pull );
   static OutputPin orange_led( GPIO_LED_BANK, Pins::pin_13, Speed::low, PullMode::pull_down, OutputMode::push_pull );
   static OutputPin red_led( GPIO_LED_BANK, Pins::pin_14, Speed::low, PullMode::pull_down, OutputMode::push_pull );
   static OutputPin blue_led( GPIO_LED_BANK, Pins::pin_15, Speed::low, PullMode::pull_down, OutputMode::push_pull );

   green_led.set( true );
   orange_led.set( true );
   red_led.set( true );
   blue_led.set( true );


   return;
}
