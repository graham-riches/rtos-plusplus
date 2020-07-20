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
	SPI_HandleTypeDef *hSpi;
	GPIO_InitTypeDef gpio;

	for ( uint8_t device = 0; device < SPI_TOTAL_DEVICES; device++ )
	{
		hSpi = &spiHandlersArray[device].handler;

		switch ( device )
		{
			case SPI_ACCELEROMETER:
			    /* Reset the CS pin */
			    HAL_GPIO_WritePin(ACCEL_CS_GPIO_BANK, ACCEL_CS_PIN, GPIO_PIN_RESET);

			    /* configure the CS GPIO pin */
                gpio.Pin = ACCEL_CS_PIN;
                gpio.Mode = GPIO_MODE_OUTPUT_PP;
                gpio.Pull = GPIO_NOPULL;
                gpio.Speed = GPIO_SPEED_FREQ_LOW;
                HAL_GPIO_Init(ACCEL_CS_GPIO_BANK, &gpio);

                /* configure the SPI parameters */
				hSpi->Instance = SPI1;
				hSpi->Init.Mode = SPI_MODE_MASTER;
				hSpi->Init.Direction = SPI_DIRECTION_2LINES;
				hSpi->Init.DataSize = SPI_DATASIZE_8BIT;
				hSpi->Init.CLKPolarity = SPI_POLARITY_LOW;
				hSpi->Init.CLKPhase = SPI_PHASE_1EDGE;
				hSpi->Init.NSS = SPI_NSS_SOFT;
				hSpi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
				hSpi->Init.FirstBit = SPI_FIRSTBIT_MSB;
				hSpi->Init.TIMode = SPI_TIMODE_DISABLE;
				hSpi->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
				hSpi->Init.CRCPolynomial = 10;
				break;

			default:
				break;
		}

		/* initialize the peripheral */
		if ( HAL_SPI_Init(hSpi) != HAL_OK )
		{
		  /*TODO: implement SPI error handler */
		}
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


/**
 * \brief read from a device over SPI
 */
void SPI_read( SPI_devices_t device, uint8_t *data, uint16_t size )
{
    if ( device >= SPI_TOTAL_DEVICES )
    {
        return;
    }
    SPI_handler_t *handler = &spiHandlersArray[device];

    /* for now use the blocking HAL read */
    HAL_SPI_Receive( &handler->handler, data, size, HAL_MAX_DELAY );
}
