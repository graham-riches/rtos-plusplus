/*! \file usart.c
*
*  \brief USART module setup and initialization.
*
*
*  \author Graham Riches
*  \copyright Copyright (c) ATTAbotics Inc. 2020
*/


/********************************** Includes *******************************************/
#include "usart.h"
#include "board.h"

/*********************************** Consts ********************************************/


/************************************ Types ********************************************/


/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/


/******************************** Local Variables **************************************/
static USART_handler_t usartHandlersArray[USART_TOTAL_DEVICES];

/****************************** Functions Prototype ************************************/


/****************************** Functions Definition ***********************************/
/**
* \name     USART_init
* \brief    Initialize the module variables and resources.
*
* \param    None
* \retval   None
*/
void USART_init( void )
{
    UART_HandleTypeDef *usart;
    GPIO_InitTypeDef gpio;

    /* common gpio parameters */
    gpio.Mode = GPIO_MODE_AF_PP;  //!< configure the GPIO pins as alternate function peripherals
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_LOW;

    for ( uint8_t device = 0; device < USART_TOTAL_DEVICES; device++ )
    {
        usart = &usartHandlersArray[device].handler;

        switch ( device )
        {
            case USART_DEBUG:
                /* enable the usart clock */
                __HAL_RCC_USART3_CLK_ENABLE();

                /* setup gpio pins */
                gpio.Pin = DEBUG_USART_TX_PIN | DEBUG_USART_RX_PIN;
                gpio.Alternate = GPIO_AF7_USART3;

                /* init the GPIO */
                HAL_GPIO_Init( GPIOD, &gpio );

                /* setup usart specifics */
                usart->Instance = DEBUG_USART_PORT;
                break;


            default:
                break;
        }

        /* common uart parameters */
        usart->Init.BaudRate = USART_BAUD_115200;
        usart->Init.WordLength = UART_WORDLENGTH_8B;
        usart->Init.StopBits = UART_STOPBITS_1;
        usart->Init.Parity = UART_PARITY_NONE;
        usart->Init.Mode = UART_MODE_TX_RX;
        usart->Init.HwFlowCtl = UART_HWCONTROL_NONE;

        if ( HAL_UART_Init( usart ) != HAL_OK )
        {
            /* TODO: hal error handler */
        }
    }
    return;
}


/**
 * \brief send data over a specific USART
 * \param data pointer to the data buffer
 * \param size how much data to send
 */
void USART_send( USART_devices_t device, uint8_t *data, uint16_t size )
{
    /* for now, just use the HAL polling mode until interrupts are setup */
    /* TODO: implement either interrupts or USART DMA */
    CHECK_ENUM_INPUT( device, USART_TOTAL_DEVICES );
    UART_HandleTypeDef *usart = &usartHandlersArray[device].handler;

    HAL_UART_Transmit( usart, data, size, HAL_MAX_DELAY );
}

