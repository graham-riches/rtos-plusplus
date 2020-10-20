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

	}
	return;
}

/**
 * \brief read/write over SPI
 * \param device the SPI peripheral device
 * \txbuff pointer to the send buffer
 * \rxBuff pointer to the receive buffer
 * \size number of bytes to send
 */
void SPI_readWrite( SPI_devices_t device, uint8_t *txBuff, uint8_t *rxBuff, uint16_t size )
{
    if ( device >= SPI_TOTAL_DEVICES )
    {
        return;
    }
    SPI_handler_t *handler = &spiHandlersArray[device];
    SPI_TypeDef *spi = handler->handler.Instance;
    uint8_t tmp = 0;

    /* enable the SPI peripheral if it's not enabled */
    __HAL_SPI_ENABLE(&handler->handler);

    /* pull the CS low */
    enableCS( handler );

    while ( size > 0 )
    {
        /* wait for the transmit buffer to clear */
        while ( READ_BIT(spi->SR, SPI_SR_TXE) == 0 ) {}

        if ( txBuff )
        {
            *((__IO uint8_t *)&(spi->DR)) = *txBuff++;
        }
        else
        {
            /* send junk for Rx purposes */
            *((__IO uint8_t *)&(spi->DR)) = (uint8_t)0x00U;
        }

        /* wait for the receive buffer to clear */
        while ( READ_BIT(spi->SR, SPI_SR_RXNE) == 0 ) {}

        /* read data in if required, otherwise throw it away (write) */
        if ( rxBuff )
        {
            *rxBuff++ = *(__IO uint8_t *)&(spi->DR);
        }
        else
        {
            tmp = *(__IO uint8_t *)&(spi->DR);
        }

        size--;
    }

    disableCS( handler );
    return;
}

/**
 * \brief write data to a SPI peripheral
 * \param device the SPI peripheral device
 * \txbuff pointer to the send buffer
 * \size number of bytes to send
 */
void SPI_write( SPI_devices_t device, uint8_t *txBuff, uint16_t size )
{
    SPI_readWrite( device, txBuff, NULL, size );
}

/**
 * \brief read data from a SPI peripheral
 * \param device the SPI peripheral device
 * \rxBuff pointer to the receive buffer
 * \size number of bytes to send
 */
void SPI_read( SPI_devices_t device, uint8_t *rxBuff, uint16_t size )
{
    SPI_readWrite( device, NULL, rxBuff, size );
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
