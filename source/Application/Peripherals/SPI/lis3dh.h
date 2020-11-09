/*! \file lis3dh.h
*
*  \brief accelerometer module functions and variables declarations.
*
*
*  \author Graham Riches
*/

#ifndef __LIS3DH_H__
#define __LIS3DH_H__


/********************************** Includes *******************************************/
#include "common.h"
#include "hal_spi.h"
#include "hal_gpio.h"
#include "stm32f4xx.h"


/*********************************** Consts ********************************************/


/************************************ Types ********************************************/

/**
 * \brief class for the LIS3DH accelerometer
 */
class LIS3DH : public HAL::SPIPolling
{
   public:
   LIS3DH( SPI_TypeDef *spi_peripheral_address, HAL::OutputPin chip_select );
   void initialize( void );
   uint16_t test( void );
};


/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/
extern LIS3DH accelerometer;

/****************************** Functions Prototype ************************************/



#endif /* __LIS3DH_H__ */
