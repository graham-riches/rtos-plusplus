/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */


#include <board.h>
#include "hal.h"


/**
  * \brief  Main application function
  * \retval int
  */
int main(void)
{
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

  }
}
