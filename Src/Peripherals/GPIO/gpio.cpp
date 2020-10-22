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
static const uint16_t ledPins[GPIO_TOTAL_LEDS] = { GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15 };

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

   /* GPIO Ports Clock Enable */
   /* TODO: remove once the HAL RCC code is built out */
   RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE );


   /* configure the pins */
   GPIO::PinConfiguration configuration;
   configuration.mode = GPIO::PinMode::output;
   configuration.output_mode = GPIO::OutputMode::push_pull;
   configuration.pull_mode = GPIO::PullMode::none;
   configuration.speed = GPIO::Speed::low;
   configuration.pins = static_cast<GPIO::Pins>( LD3_Pin | LD4_Pin | LD5_Pin | LD6_Pin );
   GPIO::initialize_pin( GPIO_LED_BANK, configuration );
   
   return;
}
