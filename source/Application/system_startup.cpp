/**
 * \file system_startup.cpp
 * \author Graham Riches
 * \brief Default system startup code
 * \version 0.1
 * \date 2022-03-21
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/********************************** Includes *******************************************/
#include "hal_flash.h"
#include "hal_interrupt.h"
#include "hal_power.h"
#include "hal_rcc.h"
#include "hal_utilities.h"
#include "os.h"

/*********************************** Consts ********************************************/
constexpr uint32_t HSE_FREQUENCY = 8000000;
constexpr uint8_t RCC_PLL_M_FACTOR = 8;
constexpr uint16_t RCC_PLL_N_FACTOR = 336;
constexpr uint8_t RCC_PLL_Q_FACTOR = 7;
constexpr HAL::PLLOutputPrescaler RCC_PLL_P_FACTOR = HAL::PLLOutputPrescaler::prescaler_2;
constexpr HAL::PLLSource RCC_PLL_SOURCE = HAL::PLLSource::high_speed_external;

/****************************** Functions Definition ***********************************/
/**
 * \brief Main system startup code, which is called before main
 * 
 */
extern "C" void __system_startup() {
    using namespace HAL;

    // Enable FPU if defined
#if ( __FPU_PRESENT == 1 ) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2)); /* set CP10 and CP11 Full Access */
#endif

    // Enable high speed external
    reset_control_clock.set_control_register(RCCRegister::hse_on, 0x01);

    // Wait for the high speed clock to stabilize
    volatile bool hse_ready = false;
    do {
        hse_ready = static_cast<bool>(reset_control_clock.get_control_register(RCCRegister::hse_ready));
    } while ( !hse_ready );

    // Select regulator voltage output Scale 1 mode
    reset_control_clock.set_apb_clock(APB1Clocks::power_management, true);
    power_management.set_control_register(PowerManagementControlRegister::voltage_output_selection, 0x03);

    // Setup the clock prescalers
    reset_control_clock.configure_ahb_clock(AHBPrescaler::prescaler_none);
    reset_control_clock.configure_apb1_clock(APBPrescaler::prescaler_4);
    reset_control_clock.configure_apb2_clock(APBPrescaler::prescaler_2);

    // Configure the main phase locked loop
    reset_control_clock.configure_main_pll(
        RCC_PLL_SOURCE, HSE_FREQUENCY, RCC_PLL_M_FACTOR, RCC_PLL_N_FACTOR, RCC_PLL_P_FACTOR, RCC_PLL_Q_FACTOR);

    // Enable the phase locked loop
    reset_control_clock.set_control_register(RCCRegister::main_pll_on, 0x01);

    // Wait for the phase locked loop to stabilize
    volatile bool pll_ready = false;
    do {
        pll_ready = static_cast<bool>(reset_control_clock.get_control_register(RCCRegister::main_pll_ready));
    } while ( pll_ready == false );

    // Configure Flash prefetch, Instruction cache, Data cache and wait state
    flash::set_access_control_register_bit(FLASH, flash::access_control_register::prefetch_enable, 0x01);
    flash::set_access_control_register_bit(FLASH, flash::access_control_register::instruction_cache_enable, 0x01);
    flash::set_access_control_register_bit(FLASH, flash::access_control_register::data_cache_enable, 0x01);
    flash::set_access_control_register_bit(FLASH, flash::access_control_register::latency, 0x05);

    // set the system clock source
    reset_control_clock.set_system_clock_source(SystemClockSource::phase_locked_loop);

    // Setup the scheduler clock ticks
    uint32_t sys_clock = reset_control_clock.get_clock_speed(Clocks::AHB1);
    os::kernel::set_systick_frequency(sys_clock / 1000);
}
