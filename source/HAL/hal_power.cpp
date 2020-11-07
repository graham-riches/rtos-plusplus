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


/*********************************** Consts ********************************************/

/************************************ Types ********************************************/

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/
PowerManagement power_management( PWR );


/******************************** Local Variables **************************************/

/****************************** Functions Prototype ************************************/

/****************************** Function Definitions ***********************************/

/**
 * \brief Construct a new Power Management:: Power Management object
 * 
 * \param pwr_peripheral_address the address of the peripheral
 */
PowerManagement::PowerManagement( PWR_TypeDef *pwr_peripheral_address )
{
   this->peripheral = pwr_peripheral_address;
}

/**
 * \brief Set the a control register
 * 
 * \param reg the register to set
 * \param value value to set
 */
void PowerManagement::set_control_register( PowerManagementControlRegister reg, uint8_t value )
{
   switch ( reg )
   {
      /* handle all single bit registers together */
      case PowerManagementControlRegister::low_power_deepsleep:
      case PowerManagementControlRegister::power_down_deepsleep:
      case PowerManagementControlRegister::clear_wakeup_flag:
      case PowerManagementControlRegister::clear_standby_flag:
      case PowerManagementControlRegister::power_voltage_detector_enable:
      case PowerManagementControlRegister::disable_backup_write_protection:
      case PowerManagementControlRegister::flash_powerdown_stop_mode:
      case PowerManagementControlRegister::low_power_regulator_deepsleep:
      case PowerManagementControlRegister::main_regulator_deepsleep:
      case PowerManagementControlRegister::adc_dc_one:
      case PowerManagementControlRegister::overdrive_enable:
      case PowerManagementControlRegister::overdrive_switching_enable:
         /* clear the register */
         this->peripheral->CR &= ~static_cast<uint8_t>( 0x01u << static_cast<uint8_t>( reg ) );
         /* set it */
         this->peripheral->CR |= static_cast<uint8_t>( ( value & 0x01u ) << static_cast<uint8_t>( reg ) );
         break;

      /* handle two bit registers together */
      case PowerManagementControlRegister::underdrive_enable:
      case PowerManagementControlRegister::voltage_output_selection:
         /* clear the register */
         this->peripheral->CR &= ~static_cast<uint8_t>( 0x03u << static_cast<uint8_t>( reg ) );
         /* set it */
         this->peripheral->CR |= static_cast<uint8_t>( ( value & 0x03 ) << static_cast<uint8_t>( reg ) );
         break;

      case PowerManagementControlRegister::power_voltage_level:
         /* clear the register */
         this->peripheral->CR &= ~static_cast<uint8_t>( 0x07u << static_cast<uint8_t>( reg ) );
         /* set it */
         this->peripheral->CR |= static_cast<uint8_t>( ( value & 0x07 ) << static_cast<uint8_t>( reg ) );
         break;

      default:
         break;
   }
}

};  // namespace HAL
