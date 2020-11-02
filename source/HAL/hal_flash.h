/*! \file hal_flash.h
*
*  \brief hal_flash module functions and variables declarations.
*
*
*  \author Graham Riches
*/

#ifndef __HAL_FLASH_H__
#define __HAL_FLASH_H__

/********************************** Includes *******************************************/
#include "hal_bitwise_operators.h"
#include "stm32f4xx.h"
#include <stdint.h>


namespace HAL
{
namespace Flash
{
/*********************************** Consts ********************************************/

/************************************ Types ********************************************/

/**
 * \brief bit offsets for the flash peripheral access control register
 * 
 */
enum class AccessControlRegister : unsigned
{
   latency = 0,
   prefetch_enable = 8,
   instruction_cache_enable = 9,
   data_cache_enable = 10,
   instruction_cache_reset = 11,
   data_cache_reset = 12
};


/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/

/****************************** Functions Prototype ************************************/
void set_access_control_register( AccessControlRegister reg,  uint8_t value );


};  // namespace Flash
};  // namespace HAL

#endif /* __HAL_FLASH_H__ */
