/*! \file gpio.c
*
*  \brief HAL GPIO drivers.
*
*
*  \author Graham Riches
*/


/********************************** Includes *******************************************/
#include "gpio.h"
#include "board.h"
#include "hal_gpio.h"
#include "hal_rcc.h"

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
   using namespace HAL;

   /* enable the peripheral clock */
   ResetControlClock::set_ahb1_clock( ResetControlClock::AHB1Clocks::gpio_d, true );

   /* configure the pins */   
   static GPIO::OutputPin green_led( GPIO_LED_BANK, GPIO::Pins::pin_12, GPIO::Speed::low, GPIO::PullMode::pull_down, GPIO::OutputMode::push_pull );
   static GPIO::OutputPin orange_led( GPIO_LED_BANK, GPIO::Pins::pin_13, GPIO::Speed::low, GPIO::PullMode::pull_down, GPIO::OutputMode::push_pull );
   static GPIO::OutputPin red_led( GPIO_LED_BANK, GPIO::Pins::pin_14, GPIO::Speed::low, GPIO::PullMode::pull_down, GPIO::OutputMode::push_pull );
   static GPIO::OutputPin blue_led( GPIO_LED_BANK, GPIO::Pins::pin_15, GPIO::Speed::low, GPIO::PullMode::pull_down, GPIO::OutputMode::push_pull );

   green_led.set( true );
   orange_led.set( true );
   red_led.set( true );
   blue_led.set( true );


   return;
}
