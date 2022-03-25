/*! \file hal_power.hpp
*
*  \brief Power management module
*
*
*  \author Graham Riches
*/

#pragma once

/********************************** Includes *******************************************/
#include "stm32f4xx.h"

namespace HAL::power_management
{
/************************************ Types ********************************************/
/**
 * \brief bit offsets for power management control register
 * 
 */
// clang-format off
enum class control_register : unsigned {
    low_power_deepsleep             = 0,
    power_down_deepsleep            = 1,
    clear_wakeup_flag               = 2,
    clear_standby_flag              = 3,
    power_voltage_detector_enable   = 4,
    power_voltage_level             = 5,
    disable_backup_write_protection = 8,
    flash_powerdown_stop_mode       = 9,
    low_power_regulator_deepsleep   = 10,
    main_regulator_deepsleep        = 11,
    adc_dc_one                      = 13,
    voltage_output_selection        = 14,
    overdrive_enable                = 16,
    overdrive_switching_enable      = 17,
    underdrive_enable               = 18
};
// clang-format on

/****************************** Function Declarations ************************************/
/**
 * \brief Set a control register value
 * 
 * \param reg The register to set
 * \param value Value to set
 */
void set_control_register(control_register reg, uint8_t value);

};  // namespace HAL
