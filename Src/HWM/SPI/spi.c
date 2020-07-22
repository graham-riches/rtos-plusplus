/*! \file spi.c
*
*  \brief SPI peripheral setup and abstraction layer.
*
*
*  \author Graham Riches
*/


/********************************** Includes *******************************************/
#include "spi.h"
#include "board.h"

/*********************************** Consts ********************************************/

/************************************ Types ********************************************/


/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/


/******************************** Local Variables **************************************/
static SPI_handler_t spiHandlersArray[SPI_TOTAL_DEVICES];

/****************************** Functions Prototype ************************************/
static void disableCS( SPI_handler_t *handler );
static void enableCS( SPI_handler_t *handler );


/****************************** Functions Definition ***********************************/
/**
* \name     SPI_init
* \brief    Initialize the module variables and resources.
*
* \param    None
* \retval   None
*/
void SPI_init( void )
{
	/* configure the general SPI peripheral parameters and any associated GPIO */
    /* NOTE:
     *  PCLK2 -> 84 MHz
     *  Prescalar Values:
     *   2 -> 42 MHz
     *   4 -> 21 MHz
     *   8 -> 10.5 MHz
     *   16 -> 5.25 MHz
     *
     */
	SPI_HandleTypeDef *hSpi;
	GPIO_InitTypeDef gpio;

	for ( uint8_t device = 0; device < SPI_TOTAL_DEVICES; device++ )
	{
		hSpi = &spiHandlersArray[device].handler;

		switch ( device )
		{
			case SPI_ACCELEROMETER:
			    /* set the CS pin and bank in the handler structure */
			    spiHandlersArray[device].bank = ACCEL_CS_GPIO_BANK;
			    spiHandlersArray[device].cs = ACCEL_CS_PIN;

                /* configure the SPI GPIO pins */
                gpio.Pin = SPI1_MOSI_Pin | SPI1_MISO_Pin | SPI1_SCK_Pin;
                gpio.Mode = GPIO_MODE_AF_PP;
                gpio.Alternate = GPIO_AF5_SPI1;
                HAL_GPIO_Init( GPIOA, &gpio );

                /* configure the SPI parameters */
				hSpi->Instance = SPI1;
				hSpi->Init.Mode = SPI_MODE_MASTER;
				hSpi->Init.Direction = SPI_DIRECTION_2LINES;
				hSpi->Init.DataSize = SPI_DATASIZE_8BIT;
				hSpi->Init.CLKPolarity = SPI_POLARITY_HIGH;
				hSpi->Init.CLKPhase = SPI_PHASE_2EDGE;
				hSpi->Init.NSS = SPI_NSS_SOFT;
				hSpi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
				hSpi->Init.FirstBit = SPI_FIRSTBIT_MSB;
				hSpi->Init.TIMode = SPI_TIMODE_DISABLE;
				hSpi->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
				hSpi->Init.CRCPolynomial = 10;

				/* enable the interrupts */
				//HAL_NVIC_SetPriority( SPI1_IRQn, 2, 2 );
                //HAL_NVIC_EnableIRQ( SPI1_IRQn );

				break;

			default:
				break;
		}

		/* initialize the SPI peripheral */
		if ( HAL_SPI_Init(hSpi) != HAL_OK )
		{
		 /*TODO: implement SPI error handler */
		}

		/* configure the CS GPIO pin */
        gpio.Pin = spiHandlersArray[device].cs;
        gpio.Mode = GPIO_MODE_OUTPUT_PP;
        gpio.Pull = GPIO_PULLUP;
        gpio.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init( spiHandlersArray[device].bank, &gpio );

        /* disable the CS for the current handler */
        disableCS( &spiHandlersArray[device] );

		/* enable the interrupts */
		//__HAL_SPI_ENABLE_IT( hSpi, SPI_IT_RXNE );
		//__HAL_SPI_ENABLE_IT( hSpi, SPI_IT_TXE );
	}
	return;
}


/**
 * \brief write to a device over SPI
 */
void SPI_write( SPI_devices_t device, uint8_t *data, uint16_t size )
{
    if ( device >= SPI_TOTAL_DEVICES )
    {
        return;
    }
    SPI_handler_t *handler = &spiHandlersArray[device];

    /* for now use the blocking HAL write */
    HAL_SPI_Transmit( &handler->handler, data, size, HAL_MAX_DELAY );

    return;
}

uint8_t SPI_readWrite( SPI_devices_t device, uint8_t addr )
{
    if ( device >= SPI_TOTAL_DEVICES )
    {
        return 0XFF;
    }
    SPI_handler_t *handler = &spiHandlersArray[device];
    SPI_TypeDef *spi = SPI1;
    __HAL_SPI_ENABLE(&handler->handler);
    uint8_t tmp;

    addr |= (uint8_t)0x80;
    while ( READ_BIT(spi->SR, SPI_SR_TXE) == 0 ) {}

    /* pull the CS low */
    enableCS( handler );

    *((__IO uint8_t *)&(SPI1->DR)) = addr;

    while ( READ_BIT(spi->SR, SPI_SR_RXNE) == 0 ) {}

    tmp = *(__IO uint8_t *)&(spi->DR);
    while ( READ_BIT(spi->SR, SPI_SR_TXE) == 0 ) {}

    *((__IO uint8_t *)&(SPI1->DR)) = (uint8_t)0x00;
    while ( READ_BIT(spi->SR, SPI_SR_RXNE) == 0 ) {}

    tmp = *(__IO uint8_t *)&(spi->DR);

    disableCS( handler );
    return tmp;
}

/**
 * \brief disable the chip select for a SPI peripheral
 */
static void disableCS( SPI_handler_t *handler )
{
    HAL_GPIO_WritePin( handler->bank, handler->cs, GPIO_PIN_SET );
}

/**
 * \brief enable the chip select for a SPI peripheral
 */
static void enableCS( SPI_handler_t *handler )
{
    HAL_GPIO_WritePin( handler->bank, handler->cs, GPIO_PIN_RESET );
}
