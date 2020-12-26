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
/*********************************** Consts ********************************************/

/************************************ Types ********************************************/

/**
 * \brief bit offsets for the flash peripheral access control register
 * 
 */
enum class FlashAccessControlRegister : unsigned {
    latency = 0,
    prefetch_enable = 8,
    instruction_cache_enable = 9,
    data_cache_enable = 10,
    instruction_cache_reset = 11,
    data_cache_reset = 12
};

/**
 * \brief flash memory peripheral class
 * 
 */
class Flash {
  private:
    FLASH_TypeDef* peripheral;

  public:
    Flash(FLASH_TypeDef* flash_peripheral_address) {
        this->peripheral = flash_peripheral_address;
    }

    void set_access_control_register(FlashAccessControlRegister reg, uint8_t value);
};

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/
extern Flash flash;

/****************************** Functions Prototype ************************************/

};  // namespace HAL

#endif /* __HAL_FLASH_H__ */
