/*! \file main.c
*
*  \brief Main application code
*  \author Graham Riches
*/


#include <board.h>
#include "hal.h"
#include "usart.h"
#include "common.h"


/**
  * \brief  Main application function
  * \retval int
  */
int main(void)
{
    char msg[] = "Hello from STM32\n";
    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* configure the project specific HAL drivers */
    HAL_moduleInit();

    /* light the LEDs */
    HAL_GPIO_WritePin( GPIOD, GPIO_PIN_12, GPIO_PIN_SET );
    HAL_GPIO_WritePin( GPIOD, GPIO_PIN_13, GPIO_PIN_SET );
    HAL_GPIO_WritePin( GPIOD, GPIO_PIN_14, GPIO_PIN_SET );
    HAL_GPIO_WritePin( GPIOD, GPIO_PIN_15, GPIO_PIN_SET );

    /* main single-threaded function */
    while (1)
    {
      USART_send( USART_DEBUG, (uint8_t *)&msg, sizeof(msg) );
      HAL_Delay( 100 );
    }
}
