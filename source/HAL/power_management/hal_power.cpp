/*! \file hal_power.cpp
*
*  \brief HAL++ imlementation of the STM32 power management.
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "hal_power.h"
#include "hal_utilities.h"

namespace HAL::power_management
{
void set_control_register(control_register reg, uint8_t value) {
    switch ( reg ) {
        // Intentional fallthrough - all single bit wide registers
        case control_register::low_power_deepsleep:
        case control_register::power_down_deepsleep:
        case control_register::clear_wakeup_flag:
        case control_register::clear_standby_flag:
        case control_register::power_voltage_detector_enable:
        case control_register::disable_backup_write_protection:
        case control_register::flash_powerdown_stop_mode:
        case control_register::low_power_regulator_deepsleep:
        case control_register::main_regulator_deepsleep:
        case control_register::adc_dc_one:
        case control_register::overdrive_enable:
        case control_register::overdrive_switching_enable:
            clear_bits(PWR->CR, 0x01u << static_cast<uint8_t>(reg));
            set_bits(PWR->CR, (value & 0x01u) << static_cast<uint8_t>(reg));
            break;

        // Intentional fallthrough - all two bit wide registers
        case control_register::underdrive_enable:
        case control_register::voltage_output_selection:
            clear_bits(PWR->CR, 0x03u << static_cast<uint8_t>(reg));
            set_bits(PWR->CR, (value & 0x03) << static_cast<uint8_t>(reg));
            break;

        case control_register::power_voltage_level:
            clear_bits(PWR->CR, 0x07u << static_cast<uint8_t>(reg));
            set_bits(PWR->CR, (value & 0x07) << static_cast<uint8_t>(reg));
            break;

        default:
            break;
    }
}

};  // namespace HAL
