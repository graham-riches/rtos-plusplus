/*! \file hal_power.cpp
*
*  \brief HAL++ imlementation of the STM32 power management.
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "hal_power.h"

namespace HAL
{
namespace PowerManagement
{

/*********************************** Consts ********************************************/

/************************************ Types ********************************************/

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/

/******************************** Local Variables **************************************/

/****************************** Functions Prototype ************************************/

/****************************** Function Definitions ***********************************/
/**
 * \brief Set the a control register
 * 
 * \param reg the register to set
 * \param value value to set
 */
void set_control_register( ControlRegister reg, uint8_t value )
{
   switch ( reg )
   {
      /* handle all single bit registers together */
      case ControlRegister::low_power_deepsleep:
      case ControlRegister::power_down_deepsleep:
      case ControlRegister::clear_wakeup_flag:
      case ControlRegister::clear_standby_flag:
      case ControlRegister::power_voltage_detector_enable:
      case ControlRegister::disable_backup_write_protection:
      case ControlRegister::flash_powerdown_stop_mode:
      case ControlRegister::low_power_regulator_deepsleep:
      case ControlRegister::main_regulator_deepsleep:
      case ControlRegister::adc_dc_one:
      case ControlRegister::overdrive_enable:
      case ControlRegister::overdrive_switching_enable:
         /* clear the register */
         PWR->CR &= ~static_cast<uint8_t>( 0x01u << static_cast<uint8_t>( reg ) );
         /* set it */
         PWR->CR |= static_cast<uint8_t>( ( value & 0x01u ) << static_cast<uint8_t>( reg ) );
         break;

      /* handle two bit registers together */
      case ControlRegister::underdrive_enable:
      case ControlRegister::voltage_output_selection:
         /* clear the register */
         PWR->CR &= ~static_cast<uint8_t>( 0x03u << static_cast<uint8_t>( reg ) );
         /* set it */
         PWR->CR |= static_cast<uint8_t>( ( value & 0x03 ) << static_cast<uint8_t>( reg ) );
         break;

      case ControlRegister::power_voltage_level:
         /* clear the register */
         PWR->CR &= ~static_cast<uint8_t>( 0x07u << static_cast<uint8_t>( reg ) );
         /* set it */
         PWR->CR |= static_cast<uint8_t>( ( value & 0x07 ) << static_cast<uint8_t>( reg ) );
         break;

      default:
         break;
   }
}

};  // namespace PowerManagement
};  // namespace HAL
