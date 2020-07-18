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
#define USART_BUFFER_SIZE 256
#define USART_RX_SIZE 1

/************************************ Types ********************************************/


/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/


/******************************** Local Variables **************************************/
static USART_handler_t usartHandlersArray[USART_TOTAL_DEVICES];


/* create a local buffer for each USART peripheral and create a ring buffer handler to manage it */
static uint8_t debugRxData[USART_BUFFER_SIZE] = {0};
static uint8_t debugTxData[USART_BUFFER_SIZE] = {0};
static UTILITIES_ringBuffer_t debugRxBuffer = { .head = 0, .tail = 0, .size = USART_BUFFER_SIZE, .data = &debugRxData };
static UTILITIES_ringBuffer_t debugTxBuffer = { .head = 0, .tail = 0, .size = USART_BUFFER_SIZE, .data = &debugTxData };

/****************************** Functions Prototype ************************************/
static void USART_IRQHandler( USART_handler_t *usart );
static USART_devices_t USART_getDevice( UART_HandleTypeDef *handle );

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
                HAL_NVIC_SetPriority( USART3_IRQn, 0, 0 );
                HAL_NVIC_EnableIRQ( USART3_IRQn );

                /* setup gpio pins */
                gpio.Pin = DEBUG_USART_TX_PIN | DEBUG_USART_RX_PIN;
                gpio.Alternate = GPIO_AF7_USART3;

                /* init the GPIO */
                HAL_GPIO_Init( GPIOD, &gpio );

                /* setup usart specifics */
                usart->Instance = DEBUG_USART_PORT;

                /* setup the ring buffer structures */
                usartHandlersArray[device].rxBuffer = &debugRxBuffer;
                usartHandlersArray[device].txBuffer = &debugTxBuffer;

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

        /* enable the rx interrupt */
        HAL_UART_Receive_IT( usart, &usartHandlersArray[device].rxDataByte, USART_RX_SIZE );
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
    USART_handler_t deviceHandler;
    UTILITIES_ringBufferStatus_t status;

    CHECK_ENUM_INPUT( device, USART_TOTAL_DEVICES );

    deviceHandler = usartHandlersArray[device];

    status = UTILITIES_ringBufferAdd( deviceHandler.txBuffer, data, size );
    if ( status != UTILITIES_RINGBUFFER_OK )
    {
        /* TODO: handle error */
        return;
    }

    /* set the data available interrupt bit */
    SET_BIT( deviceHandler.handler.Instance->CR1, USART_CR1_TCIE );


    return;
}


/****************************** USART Interrupt Functions ***********************************/
/**
 * \brief generic callback for all UART interrupts
 */
static void USART_IRQHandler( USART_handler_t *usart )
{
    /* pass the peripheral handle pointer to the HAL ISR handler */
    HAL_UART_IRQHandler( &usart->handler );
}


/**
 * \brief get the local handler structure given a peripheral handle pointer
 * \retval index if found, error otherwise
 */
static USART_devices_t USART_getDevice( UART_HandleTypeDef *handle )
{
    USART_devices_t index;
    for ( index = 0; index < USART_TOTAL_DEVICES; index++ )
    {
        if ( handle == &usartHandlersArray[index].handler )
        {
            /* found the corresponding peripheral */
            return index;
        }
    }
    return USART_INVALID_DEVICE;
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


/**
 * \brief generic HAL callback function for data received interrupt
 */
void HAL_UART_RxCpltCallback( UART_HandleTypeDef *handle )
{
    UTILITIES_ringBufferStatus_t status;

    /* get the appropriate handler and pass the data to it's ring buffer */
    USART_devices_t device = USART_getDevice( handle );
    USART_handler_t deviceHandler;
    if ( device == USART_INVALID_DEVICE )
    {
        return;
    }
    deviceHandler = usartHandlersArray[device];

    /* receive the data */
    status = UTILITIES_ringBufferAdd( deviceHandler.rxBuffer, &deviceHandler.rxDataByte, USART_RX_SIZE );
    if ( status != UTILITIES_RINGBUFFER_OK )
    {
        /* TODO: handle error */
    }

    /* re-enable the interrupt */
    HAL_UART_Receive_IT( &deviceHandler.handler, &deviceHandler.rxDataByte, USART_RX_SIZE );
    return;
}


/**
 * \brief generic HAL callback function for data transmit interrupt
 */
void HAL_UART_TxCpltCallback( UART_HandleTypeDef *handle )
{
    USART_devices_t device = USART_getDevice( handle );
    USART_handler_t deviceHandler;
    uint8_t buffer[USART_BUFFER_SIZE] = {0};
    uint16_t size;
    if ( device == USART_INVALID_DEVICE )
    {
        return;
    }
    deviceHandler = usartHandlersArray[device];

    size = UTILITIES_ringBufferGetUsedSpace( deviceHandler.txBuffer );
    if ( size > USART_BUFFER_SIZE )
    {
        return;
    }

    UTILITIES_ringBufferGet( deviceHandler.txBuffer, buffer, size );

    /* send it */
    HAL_StatusTypeDef retval = HAL_UART_Transmit_IT( &deviceHandler.handler, buffer, size );
    return;
}

