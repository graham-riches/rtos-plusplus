/*! \file hal_power.h
*
*  \brief hal_power module functions and variables declarations.
*
*
*  \author Graham Riches
*/

#ifndef __HAL_POWER_H__
#define __HAL_POWER_H__

/********************************** Includes *******************************************/
#include "hal_bitwise_operators.h"
#include "stm32f4xx.h"
#include <stdint.h>

namespace HAL
{

/*********************************** Consts ********************************************/

/************************************ Types ********************************************/
/**
 * \brief bit offsets for power management control register
 * 
 */
enum class PowerManagementControlRegister : unsigned {
    low_power_deepsleep = 0,
    power_down_deepsleep = 1,
    clear_wakeup_flag = 2,
    clear_standby_flag = 3,
    power_voltage_detector_enable = 4,
    power_voltage_level = 5,
    disable_backup_write_protection = 8,
    flash_powerdown_stop_mode = 9,
    low_power_regulator_deepsleep = 10,
    main_regulator_deepsleep = 11,
    adc_dc_one = 13,
    voltage_output_selection = 14,
    overdrive_enable = 16,
    overdrive_switching_enable = 17,
    underdrive_enable = 18
};

/**
 * \brief power management peripheral class
 * 
 */
class PowerManagement {
  private:
    PWR_TypeDef* peripheral;

  public:
    PowerManagement(PWR_TypeDef* pwr_peripheral_address);
    void set_control_register(PowerManagementControlRegister reg, uint8_t value);
};

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/
extern PowerManagement power_management;

/****************************** Functions Prototype ************************************/

};  // namespace HAL

#endif /* __HAL_POWER_H__ */
