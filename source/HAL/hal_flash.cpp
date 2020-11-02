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
namespace Flash
{
/*********************************** Consts ********************************************/

/************************************ Types ********************************************/

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/

/******************************** Local Variables **************************************/

/****************************** Functions Prototype ************************************/

/****************************** Function Definitions ***********************************/
/**
 * \brief configure a register in the flash access control register
 * 
 * \param value the value of the register
 */
void set_access_control_register( AccessControlRegister reg,  uint8_t value )
{
   switch ( reg )
   {
      /* handle the single bit registers in one case */
      case AccessControlRegister::data_cache_enable:
      case AccessControlRegister::prefetch_enable:
      case AccessControlRegister::instruction_cache_enable:
      case AccessControlRegister::data_cache_reset:
      case AccessControlRegister::instruction_cache_reset:
         /* clear the register */
         FLASH->ACR &= ~static_cast<uint8_t>( 0x01u << static_cast<uint8_t>( reg ) );
         /* set it */
         FLASH->ACR |= static_cast<uint8_t>( ( value & 0x01u) << static_cast<uint8_t>( reg ) );
         break;
      
      /* handle the latency (4-bits) separately */
      case AccessControlRegister::latency:
         /* clear the register */
         FLASH->ACR &= ~static_cast<uint8_t>( 0x0Fu << static_cast<uint8_t>( reg ) );
         /* set it */
         FLASH->ACR |= static_cast<uint8_t>( ( value & 0x0Fu) << static_cast<uint8_t>( reg ) );
         break;

      default:
         break;
   }
}


};  // namespace Flash
};  // namespace HAL
