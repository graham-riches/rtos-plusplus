/*! \file hal_flash.cpp
*
*  \brief HAL++ imlementation of the STM32 flash peripheral.
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "hal_flash.h"

namespace HAL
{
/*********************************** Consts ********************************************/

/************************************ Types ********************************************/

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/
Flash flash(FLASH);

/******************************** Local Variables **************************************/

/****************************** Functions Prototype ************************************/

/****************************** Function Definitions ***********************************/
/**
 * \brief configure a register in the flash access control register
 * 
 * \param value the value of the register
 */
void Flash::set_access_control_register(FlashAccessControlRegister reg, uint8_t value) {
    switch ( reg ) {
        /* handle the single bit registers in one case */
        case FlashAccessControlRegister::data_cache_enable:
        case FlashAccessControlRegister::prefetch_enable:
        case FlashAccessControlRegister::instruction_cache_enable:
        case FlashAccessControlRegister::data_cache_reset:
        case FlashAccessControlRegister::instruction_cache_reset:
            /* clear the register */
            this->peripheral->ACR &= ~static_cast<uint8_t>(0x01u << static_cast<uint8_t>(reg));
            /* set it */
            this->peripheral->ACR |= static_cast<uint8_t>((value & 0x01u) << static_cast<uint8_t>(reg));
            break;

        /* handle the latency (4-bits) separately */
        case FlashAccessControlRegister::latency:
            /* clear the register */
            this->peripheral->ACR &= ~static_cast<uint8_t>(0x0Fu << static_cast<uint8_t>(reg));
            /* set it */
            this->peripheral->ACR |= static_cast<uint8_t>((value & 0x0Fu) << static_cast<uint8_t>(reg));
            break;

        default:
            break;
    }
}

};  // namespace HAL
