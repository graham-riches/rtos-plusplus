// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Graham Riches

#pragma once

/********************************** Includes *******************************************/
#include "stm32f4xx.h"

/*********************************** Consts ********************************************/
#define PC14_OSC32_IN_Pin GPIO_Pin_14
#define PC14_OSC32_IN_GPIO_Port GPIOC
#define PC15_OSC32_OUT_Pin GPIO_Pin_15
#define PC15_OSC32_OUT_GPIO_Port GPIOC
#define PH0_OSC_IN_Pin GPIO_Pin_0
#define PH0_OSC_IN_GPIO_Port GPIOH
#define PH1_OSC_OUT_Pin GPIO_Pin_1
#define PH1_OSC_OUT_GPIO_Port GPIOH
#define OTG_FS_PowerSwitchOn_Pin GPIO_Pin_0
#define OTG_FS_PowerSwitchOn_GPIO_Port GPIOC
#define PDM_OUT_Pin GPIO_Pin_3
#define PDM_OUT_GPIO_Port GPIOC
#define B1_Pin GPIO_Pin_0
#define B1_GPIO_Port GPIOA
#define I2S3_WS_Pin GPIO_Pin_4
#define I2S3_WS_GPIO_Port GPIOA
#define BOOT1_Pin GPIO_Pin_2
#define BOOT1_GPIO_Port GPIOB
#define CLK_IN_Pin GPIO_Pin_10
#define CLK_IN_GPIO_Port GPIOB
#define LD4_Pin GPIO_Pin_12
#define LD4_GPIO_Port GPIOD
#define LD3_Pin GPIO_Pin_13
#define LD3_GPIO_Port GPIOD
#define LD5_Pin GPIO_Pin_14
#define LD5_GPIO_Port GPIOD
#define LD6_Pin GPIO_Pin_15
#define LD6_GPIO_Port GPIOD
#define I2S3_MCK_Pin GPIO_Pin_7
#define I2S3_MCK_GPIO_Port GPIOC
#define VBUS_FS_Pin GPIO_Pin_9
#define VBUS_FS_GPIO_Port GPIOA
#define OTG_FS_ID_Pin GPIO_Pin_10
#define OTG_FS_ID_GPIO_Port GPIOA
#define OTG_FS_DM_Pin GPIO_Pin_11
#define OTG_FS_DM_GPIO_Port GPIOA
#define OTG_FS_DP_Pin GPIO_Pin_12
#define OTG_FS_DP_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_Pin_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_Pin_14
#define SWCLK_GPIO_Port GPIOA
#define I2S3_SCK_Pin GPIO_Pin_10
#define I2S3_SCK_GPIO_Port GPIOC
#define I2S3_SD_Pin GPIO_Pin_12
#define I2S3_SD_GPIO_Port GPIOC
#define Audio_RST_Pin GPIO_Pin_4
#define Audio_RST_GPIO_Port GPIOD
#define OTG_FS_OverCurrent_Pin GPIO_Pin_5
#define OTG_FS_OverCurrent_GPIO_Port GPIOD
#define SWO_Pin GPIO_Pin_3
#define SWO_GPIO_Port GPIOB
#define Audio_SCL_Pin GPIO_Pin_6
#define Audio_SCL_GPIO_Port GPIOB
#define Audio_SDA_Pin GPIO_Pin_9
#define Audio_SDA_GPIO_Port GPIOB
#define MEMS_INT2_Pin GPIO_Pin_1
#define MEMS_INT2_GPIO_Port GPIOE

/* USART2 - Debug Port */
#define DEBUG_USART_PORT USART2
#define DEBUG_USART_TX_PORT GPIOA
#define DEBUG_USART_RX_PORT GPIOA
#define DEBUG_USART_TX_PIN GPIO_Pin_2
#define DEBUG_USART_RX_PIN GPIO_Pin_3

/* MEMS Accelerometer Pin Definitions */
#define ACCEL_CS_PIN GPIO_Pin_3
#define ACCEL_CS_GPIO_BANK GPIOE
#define ACCEL_DR_INT_PIN GPIO_Pin_0
#define ACCEL_DR_INT_BANK GPIOE

/* SPI1 Bus */
#define SPI1_SCK_Pin GPIO_Pin_5
#define SPI1_SCK_GPIO_Port GPIOA
#define SPI1_MISO_Pin GPIO_Pin_6
#define SPI1_MISO_GPIO_Port GPIOA
#define SPI1_MOSI_Pin GPIO_Pin_7
#define SPI1_MOSI_GPIO_Port GPIOA

/* Audio in ADC */
#define AUDIO_IN_ADC ADC1
#define AUDIO_IN_ADC_CHANNEL ADC_CHANNEL_1
#define AUDIO_IN_PIN GPIO_Pin_1
#define AUDIO_IN_PORT GPIOA

