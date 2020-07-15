/*! \file spi.c
*
*  \brief SPI peripheral setup and abstraction layer.
*
*
*  \author Graham Riches
*/


/********************************** Includes *******************************************/
#include "spi.h"

/*********************************** Consts ********************************************/

/************************************ Types ********************************************/


/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/


/******************************** Local Variables **************************************/
static SPI_handler_t spiHandlersArray[SPI_MAX_DEVICES];

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
	/* configure the general SPI peripheral parameters */
	SPI_HandleTypeDef *hSpi;
	for ( uint8_t device = 0; device < SPI_MAX_DEVICES; device++ )
	{
		hSpi = &spiHandlersArray[device].handler;

		switch ( device )
		{
			case SPI_DEFAULT_DEVICE:
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
