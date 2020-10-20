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
   GPIO_InitTypeDef GPIO_InitStruct = {0};

   /* GPIO Ports Clock Enable */
   RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE );
   RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE );
   RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC, ENABLE );
   RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD, ENABLE );
   RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOE, ENABLE );
   RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOH, ENABLE );


   /*Configure GPIO pin Output Level */
   // HAL_GPIO_WritePin(OTG_FS_PowerSwitchOn_GPIO_Port, OTG_FS_PowerSwitchOn_Pin, GPIO_PIN_SET);

   /*Configure GPIO pin Output Level */
   /*
   HAL_GPIO_WritePin(GPIOD, LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin
                           |Audio_RST_Pin, GPIO_PIN_RESET);
   */                        

   /*Configure GPIO pin : OTG_FS_PowerSwitchOn_Pin */
   /*
   GPIO_InitStruct.Pin = OTG_FS_PowerSwitchOn_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
   HAL_GPIO_Init(OTG_FS_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);
   */

   /*Configure GPIO pin : PDM_OUT_Pin */
   /*
   GPIO_InitStruct.Pin = PDM_OUT_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
   GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
   HAL_GPIO_Init(PDM_OUT_GPIO_Port, &GPIO_InitStruct);
   */

   /*Configure GPIO pin : B1_Pin */
   /*   
   GPIO_InitStruct.Pin = B1_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);
   */
   

   /*Configure GPIO pin : BOOT1_Pin */
   /*
   GPIO_InitStruct.Pin = BOOT1_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   HAL_GPIO_Init(BOOT1_GPIO_Port, &GPIO_InitStruct);
   */

   /*Configure GPIO pin : CLK_IN_Pin */
   /*
   GPIO_InitStruct.Pin = CLK_IN_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
   GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
   HAL_GPIO_Init(CLK_IN_GPIO_Port, &GPIO_InitStruct);
   */

   /*Configure GPIO pins : LD4_Pin LD3_Pin LD5_Pin LD6_Pin
                           Audio_RST_Pin */
   GPIO_InitStruct.GPIO_Pin = LD3_Pin | LD4_Pin | LD5_Pin | LD6_Pin;
   GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
   GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_InitStruct.GPIO_Speed = GPIO_Low_Speed;                     

   GPIO_Init( GPIO_LED_BANK, &GPIO_InitStruct);
   

   /*Configure GPIO pin : OTG_FS_OverCurrent_Pin */
   /*
   GPIO_InitStruct.Pin = OTG_FS_OverCurrent_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   HAL_GPIO_Init(OTG_FS_OverCurrent_GPIO_Port, &GPIO_InitStruct);
   */

   /*Configure GPIO pin : MEMS_INT2_Pin */
   /*
   GPIO_InitStruct.Pin = MEMS_INT2_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   HAL_GPIO_Init(MEMS_INT2_GPIO_Port, &GPIO_InitStruct);
   */

   /* configure the data ready interrupt from the accelerometer */
   /*
   GPIO_InitStruct.Pin = ACCEL_DR_INT_PIN;
   GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   HAL_GPIO_Init(ACCEL_DR_INT_BANK, &GPIO_InitStruct);
   */


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
      GPIO_SetBits( GPIO_LED_BANK, ledPins[led] );
   }
   else
   {
      GPIO_ResetBits( GPIO_LED_BANK, ledPins[led] );
   }   
   return;
}
