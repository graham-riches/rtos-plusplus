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
#include "event.h"

/*********************************** Consts ********************************************/
#define USART_BUFFER_SIZE 256
#define USART_RX_SIZE 1
#define USART_TX_SIZE 1

/************************************ Types ********************************************/


/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/


/******************************** Local Variables **************************************/
static USART_handler_t usartHandlersArray[USART_TOTAL_DEVICES];


/* create a local buffer for each USART peripheral and create a ring buffer handler to manage it */
static uint8_t debugRxData[USART_BUFFER_SIZE] = {0};
static uint8_t debugTxData[USART_BUFFER_SIZE] = {0};
static UTILITIES_ringBuffer_t debugRxBuffer = { .head = 0, .tail = 0, .size = USART_BUFFER_SIZE, .data = debugRxData };
static UTILITIES_ringBuffer_t debugTxBuffer = { .head = 0, .tail = 0, .size = USART_BUFFER_SIZE, .data = debugTxData };

/****************************** Functions Prototype ************************************/
static void USART_IRQHandler( USART_handler_t *usart );

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

                /* set the interrupt priority and enable the interrupt */
                HAL_NVIC_SetPriority( USART3_IRQn, 3, 3 );
                HAL_NVIC_EnableIRQ( USART3_IRQn );

                /* setup gpio pins */
                gpio.Pin = DEBUG_USART_TX_PIN | DEBUG_USART_RX_PIN;
                gpio.Alternate = GPIO_AF7_USART3;

                /* init the GPIO */
                HAL_GPIO_Init( GPIOD, &gpio );

                /* setup usart specifics */
                usart->Instance = DEBUG_USART_PORT;

                /* setup the ring buffer structures and event flags */
                usartHandlersArray[device].rxBuffer = &debugRxBuffer;
                usartHandlersArray[device].txBuffer = &debugTxBuffer;
                usartHandlersArray[device].eventFlagsGroup = &mainEventFlags;
                usartHandlersArray[device].eventFlagBit = EVENT_USART_DEBUG_RX;

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
        usart->Init.OverSampling = UART_OVERSAMPLING_16;

        if ( HAL_UART_Init( usart ) != HAL_OK )
        {
            /* TODO: hal error handler */
        }

        /* enable the rx interrupt */
        __HAL_UART_ENABLE_IT( usart, UART_IT_RXNE );
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
    USART_handler_t *deviceHandler;
    UTILITIES_ringBufferStatus_t status;

    if ( device >= USART_TOTAL_DEVICES )
    {
        return;
    }

    deviceHandler = &usartHandlersArray[device];

    status = UTILITIES_ringBufferAdd( deviceHandler->txBuffer, data, size );
    if ( status != UTILITIES_RINGBUFFER_OK )
    {
        /* TODO: handle error */
        return;
    }

    /* set the data to send counter */
    deviceHandler->handler.TxXferCount += size;

    /* set the data available interrupt bit */
    __HAL_UART_ENABLE_IT( &deviceHandler->handler, UART_IT_TXE );

    return;
}


/**
 * \brief receive data on a USART peripheral
 * \param device the UART peripheral to receive on
 * \data pointer to the receiver data buffer
 * \maxSize maxSize of the receivers buffer
 * \retval number of bytes received from the buffer
 */
uint16_t USART_recv( USART_devices_t device, uint8_t *data, uint16_t maxSize )
{
    if ( device >= USART_TOTAL_DEVICES )
    {
        return 0;
    }
    USART_handler_t *deviceHandler;
    deviceHandler = &usartHandlersArray[device];

    uint16_t bytesReceived = 0;

    if ( UTILITIES_ringBufferGetUsedSpace(deviceHandler->rxBuffer) <= maxSize )
    {
        bytesReceived = UTILITIES_ringBufferGet( deviceHandler->rxBuffer, data, maxSize );
    }
    return bytesReceived;
}


/****************************** USART Interrupt Functions ***********************************/
/**
 * \brief generic callback for all UART interrupts
 */
static void USART_IRQHandler( USART_handler_t *usart )
{
    uint32_t isrflags   = usart->handler.Instance->SR;
    uint32_t cr1its     = usart->handler.Instance->CR1;
    uint32_t dr         = usart->handler.Instance->DR;
    uint32_t errorflags = 0x00U;
    uint8_t data = 0;

    DISABLE_INTERRUPTS();

    /* check for error flags */
    errorflags = (isrflags & (uint32_t)(USART_SR_PE | USART_SR_FE | USART_SR_ORE | USART_SR_NE));
    if ( errorflags )
    {
        /* TODO: handle error */
    }
    else
    {
        /* handle a received data interrupt */
        if ( (isrflags & USART_SR_RXNE) && (cr1its & USART_CR1_RXNEIE) )
        {
            /* copy the received byte into the handlers ring buffer */
            data = (uint8_t)(dr & 0xFF);
            UTILITIES_ringBufferAdd( usart->rxBuffer, &data, USART_RX_SIZE );
            /* set the data received event */
            if ( usart->eventFlagsGroup )
            {
                EVENT_set( usart->eventFlagsGroup, usart->eventFlagBit );
            }
        }
        /* handle a data transmit interrupt */
        if ( (cr1its & USART_CR1_TXEIE) && (isrflags & USART_SR_TXE) )
        {
            UTILITIES_ringBufferGet( usart->txBuffer, &data, USART_TX_SIZE );
            usart->handler.Instance->DR = (uint8_t)((0x000000FF) & data);

            if ( --usart->handler.TxXferCount == 0U )
            {
                /* Disable the UART Transmit Complete Interrupt */
                __HAL_UART_DISABLE_IT( &usart->handler, UART_IT_TXE);
            }
        }
    }

    ENABLE_INTERRUPTS();

    return;
}


/**
 * \brief ISR for USART3
 * \note this is defined as .weak in startup_stm32f407xx.s
 */
void USART3_IRQHandler( void )
{
    USART_IRQHandler( &usartHandlersArray[USART_DEBUG] );
    return;
}
