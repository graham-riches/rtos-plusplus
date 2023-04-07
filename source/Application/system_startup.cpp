// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Graham Riches

/********************************** Includes *******************************************/
#include "hal_flash.hpp"
#include "hal_nvic.hpp"
#include "hal_power.hpp"
#include "hal_rcc.hpp"
#include "hal_utilities.hpp"
#include "os.hpp"

/*********************************** Consts ********************************************/
constexpr uint32_t HSE_FREQUENCY = 8000000;
constexpr uint8_t RCC_PLL_M_FACTOR = 8;
constexpr uint16_t RCC_PLL_N_FACTOR = 336;
constexpr uint8_t RCC_PLL_Q_FACTOR = 7;
constexpr HAL::rcc::pll_output_prescaler RCC_PLL_P_FACTOR = HAL::rcc::pll_output_prescaler::prescaler_2;
constexpr HAL::rcc::pll_source RCC_PLL_SOURCE = HAL::rcc::pll_source::high_speed_external;

/****************************** Functions Definition ***********************************/
/**
 * \brief Main system startup code, which is called before main
 * 
 */
extern "C" void __system_startup() {
    using namespace HAL;

    // Enable FPU if defined
#if defined(__FPU_PRESENT) && defined(__FPU_USED)
    // Set CP10/11 for full access to FPU
    set_bits(SCB->CPACR, (3UL << 10 * 2) | (3UL << 11 * 2));    
#endif

    // Enable various fault interrupts   
    clear_bits(FPU->FPCCR, FPU_FPCCR_LSPEN_Msk); // disable lazy stacking
    clear_bits(FPU->FPCCR, FPU_FPCCR_ASPEN_Msk);
    set_bits(SCB->SHCSR, SCB_SHCSR_MEMFAULTENA_Msk);
    set_bits(SCB->SHCSR, SCB_SHCSR_BUSFAULTENA_Msk);
    set_bits(SCB->SHCSR, SCB_SHCSR_USGFAULTENA_Msk);

    // Setup NVIC Defaults
    nvic::initialize();

    // Enable high speed external
    rcc::set_control_register(rcc::control_register::hse_on, 0x01);

    // Wait for the high speed clock to stabilize
    volatile bool hse_ready = false;
    do {
        hse_ready = static_cast<bool>(rcc::get_control_register(rcc::control_register::hse_ready));
    } while ( !hse_ready );

    // Select regulator voltage output Scale 1 mode
    rcc::set_apb_clock(rcc::apb1_clocks::power_management, true);
    power_management::set_control_register(power_management::control_register::voltage_output_selection, 0x03);

    // Setup the clock prescalers
    rcc::configure_ahb_clock(rcc::ahb_prescaler::prescaler_none);
    rcc::configure_apb1_clock(rcc::apb_prescaler::prescaler_4);
    rcc::configure_apb2_clock(rcc::apb_prescaler::prescaler_2);

    // Configure the main phase locked loop
    rcc::configure_main_pll(RCC_PLL_SOURCE, HSE_FREQUENCY, RCC_PLL_M_FACTOR, RCC_PLL_N_FACTOR, RCC_PLL_P_FACTOR, RCC_PLL_Q_FACTOR);

    // Enable the phase locked loop
    rcc::set_control_register(rcc::control_register::main_pll_on, 0x01);

    // Wait for the phase locked loop to stabilize
    volatile bool pll_ready = false;
    do {
        pll_ready = static_cast<bool>(rcc::get_control_register(rcc::control_register::main_pll_ready));
    } while ( pll_ready == false );

    // Configure Flash prefetch, Instruction cache, Data cache and wait state
    flash::set_access_control_register_bit(flash::access_control_register::prefetch_enable, 0x01);
    flash::set_access_control_register_bit(flash::access_control_register::instruction_cache_enable, 0x01);
    flash::set_access_control_register_bit(flash::access_control_register::data_cache_enable, 0x01);
    flash::set_access_control_register_bit(flash::access_control_register::latency, 0x05);

    // set the system clock source
    rcc::set_system_clock_source(rcc::system_clock_source::phase_locked_loop);

    // Setup the scheduler clock ticks
    uint32_t sys_clock = rcc::get_clock_speed(rcc::clocks::AHB1);
    os::kernel::set_systick_frequency(sys_clock / 1000);
}
