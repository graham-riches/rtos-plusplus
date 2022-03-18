/*! \file peripherals.cpp
*
*  \brief HAL setup and abstraction for all peripherals
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "peripherals.h"
#include "debug_port.h"
#include "hal_flash.h"
#include "hal_interrupt.h"
#include "hal_power.h"
#include "hal_rcc.h"
#include "lis3dsh.h"
#include "os.h"


/*********************************** Consts ********************************************/
constexpr uint32_t HSE_FREQUENCY = 8000000;
constexpr uint8_t RCC_PLL_M_FACTOR = 8;
constexpr uint16_t RCC_PLL_N_FACTOR = 336;
constexpr uint8_t RCC_PLL_Q_FACTOR = 7;
constexpr HAL::PLLOutputPrescaler RCC_PLL_P_FACTOR = HAL::PLLOutputPrescaler::prescaler_2;
constexpr HAL::PLLSource RCC_PLL_SOURCE = HAL::PLLSource::high_speed_external;

/************************************ Types ********************************************/

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/
HAL::OutputPin green_led(GPIOD, HAL::Pins::pin_12, HAL::PinMode::output, HAL::Speed::low, HAL::PullMode::pull_down, HAL::OutputMode::push_pull);
HAL::OutputPin orange_led(GPIOD, HAL::Pins::pin_13, HAL::PinMode::output, HAL::Speed::low, HAL::PullMode::pull_down, HAL::OutputMode::push_pull);
HAL::OutputPin red_led(GPIOD, HAL::Pins::pin_14, HAL::PinMode::output, HAL::Speed::low, HAL::PullMode::pull_down, HAL::OutputMode::push_pull);
HAL::OutputPin blue_led(GPIOD, HAL::Pins::pin_15, HAL::PinMode::output, HAL::Speed::low, HAL::PullMode::pull_down, HAL::OutputMode::push_pull);

/******************************** Local Variables **************************************/

/****************************** Functions Prototype ************************************/
void system_initialize_fpu(void);
void system_boot(void);

/****************************** Functions Definition ***********************************/
/**
* \brief    setup all of the peripherals
*
* \param    None
* \retval   None
*/
void initialize_peripherals(void) {
    /* enable the FPU if required */
    system_initialize_fpu();

    /* boot-up the system - RCC, Flash etc. */
    system_boot();

    /* initialize the debug UART and print a welcome banner */
    //debug_port.initialize();

    /* print out a splash screen */
    //debug_port.send("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    //debug_port.send("STM32F407 Debug Terminal \n");
    //debug_port.send("   Author: Graham Riches \n");
    //debug_port.send("   Date: November 1, 2020 \n");
    //debug_port.send("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");    

    /* turn on the LEDs for status indication */
    green_led.set(true);
    orange_led.set(false);
    red_led.set(true);
    blue_led.set(false);

    /* initialize the accelerometer */
    //accelerometer.initialize();
}

/**
 * \brief handle the system bootup
 * \note this is a fairly complex sequence:
 *    - enable HSE and wait for it to be ready,
 *    - set the power mode of the internal voltage regulator
 *    - configure the main PLL
 *    - start the main PLL and wait for it to be ready
 *    - configure the flash prefetch, etc.
 *    - set the main system clock to be driven by the PLL
 */
void system_boot(void) {
    /* enable the high speed external clock source */
    HAL::reset_control_clock.set_control_register(HAL::RCCRegister::hse_on, 0x01);

    /* wait for the high speed clock to stabilize */
    volatile bool hse_ready = false;
    do {
        hse_ready = static_cast<bool>(HAL::reset_control_clock.get_control_register(HAL::RCCRegister::hse_ready));
    } while ( hse_ready == false );

    /* Select regulator voltage output Scale 1 mode */
    HAL::reset_control_clock.set_apb_clock(HAL::APB1Clocks::power_management, true);
    HAL::power_management.set_control_register(HAL::PowerManagementControlRegister::voltage_output_selection, 0x03);

    /* setup the clock prescalers */
    HAL::reset_control_clock.configure_ahb_clock(HAL::AHBPrescaler::prescaler_none);
    HAL::reset_control_clock.configure_apb1_clock(HAL::APBPrescaler::prescaler_4);
    HAL::reset_control_clock.configure_apb2_clock(HAL::APBPrescaler::prescaler_2);

    /* configure the main phase locked loop */
    HAL::reset_control_clock.configure_main_pll(RCC_PLL_SOURCE, HSE_FREQUENCY, RCC_PLL_M_FACTOR, RCC_PLL_N_FACTOR, RCC_PLL_P_FACTOR, RCC_PLL_Q_FACTOR);

    /* enable the phase locked loop */
    HAL::reset_control_clock.set_control_register(HAL::RCCRegister::main_pll_on, 0x01);

    /* wait for the phase locked loop to stabilize */
    volatile bool pll_ready = false;
    do {
        pll_ready = static_cast<bool>(HAL::reset_control_clock.get_control_register(HAL::RCCRegister::main_pll_ready));
    } while ( pll_ready == false );

    /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
    HAL::flash.set_access_control_register(HAL::FlashAccessControlRegister::prefetch_enable, 0x01);
    HAL::flash.set_access_control_register(HAL::FlashAccessControlRegister::instruction_cache_enable, 0x01);
    HAL::flash.set_access_control_register(HAL::FlashAccessControlRegister::data_cache_enable, 0x01);
    HAL::flash.set_access_control_register(HAL::FlashAccessControlRegister::latency, 0x05);

    /* set the system clock source */
    HAL::reset_control_clock.set_system_clock_source(HAL::SystemClockSource::phase_locked_loop);

    //!< setup the scheduler clock ticks
    uint32_t sys_clock = HAL::reset_control_clock.get_clock_speed(HAL::Clocks::AHB1);
    os::kernel::set_systick_frequency(sys_clock / 1000);
}

/**
 * \brief enable the floating point unit if required
 * \todo this needs to be moved somewhere else
 */
void system_initialize_fpu(void) {
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2)); /* set CP10 and CP11 Full Access */
#endif
}
