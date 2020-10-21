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
   //GPIO_InitTypeDef GPIO_InitStruct = {0};

   /* GPIO Ports Clock Enable */
   /* TODO: remove once the HAL RCC code is built out */
   RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE );


   /*Configure GPIO pins : LD4_Pin LD3_Pin LD5_Pin LD6_Pin
                           Audio_RST_Pin */
   GPIO_InitStruct.GPIO_Pin = LD3_Pin | LD4_Pin | LD5_Pin | LD6_Pin;
   GPIO_InitStruct.GPIO_Mode = GPIO::PinMode::output;
   GPIO_InitStruct.GPIO_OType = GPIO::OutputMode::push_pull;
   GPIO_InitStruct.GPIO_PuPd = GPIO::PullMode::none;
   GPIO_InitStruct.GPIO_Speed = GPIO::Speed::low;                     

   GPIO_Init( GPIO_LED_BANK, &GPIO_InitStruct);
   
   return;
}


/**
 * \brief set an LED pin state
 */
void GPIO_setLED( GPIO_LED_t led, GPIO_LED_state_t state )
{
   if ( led >= GPIO_TOTAL_LEDS )
   {
      return;
   }
   if ( state )
   {
      //GPIO_SetBits( GPIO_LED_BANK, ledPins[led] );
   }
   else
   {
      //GPIO_ResetBits( GPIO_LED_BANK, ledPins[led] );
   }   
   return;
}
