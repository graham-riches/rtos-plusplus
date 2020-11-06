/*! \file peripherals.cpp
*
*  \brief HAL setup and abstraction for all peripherals
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "peripherals.h"
#include "gpio.h"
#include "hal_flash.h"
#include "hal_interrupt.h"
#include "hal_power.h"
#include "hal_rcc.h"
#include "usart.h"

/*********************************** Consts ********************************************/
constexpr uint32_t HSE_FREQUENCY = 8000000;
constexpr uint8_t RCC_PLL_M_FACTOR = 8;
constexpr uint16_t RCC_PLL_N_FACTOR = 336;
constexpr uint8_t RCC_PLL_Q_FACTOR = 7;
constexpr HAL::ResetControlClock::PLLOutputPrescaler RCC_PLL_P_FACTOR = HAL::ResetControlClock::PLLOutputPrescaler::prescaler_2;
constexpr HAL::ResetControlClock::PLLSource RCC_PLL_SOURCE = HAL::ResetControlClock::PLLSource::high_speed_external;

/************************************ Types ********************************************/

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/

/******************************** Local Variables **************************************/

/****************************** Functions Prototype ************************************/
void system_boot( void );

/****************************** Functions Definition ***********************************/
/**
* \brief    setup all of the peripherals
*
* \param    None
* \retval   None
*/
void initialize_peripherals( void )
{
   /* boot-up the system - RCC, Flash etc. */
   system_boot( );

   /* Initialize all configured peripherals */
   GPIO_initialize( );

   /* enable the debug usart */
   USART_initialize( );
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
void system_boot( void )
{
   /* enable the high speed external clock source */
   HAL::ResetControlClock::set_control_register( HAL::ResetControlClock::RCCRegister::hse_on, 0x01 );

   /* wait for the high speed clock to stabilize */
   volatile bool hse_ready = false;
   do
   {
      hse_ready = static_cast<bool>( HAL::ResetControlClock::get_control_register( HAL::ResetControlClock::RCCRegister::hse_ready ) );
   } while ( hse_ready == false );

   /* Select regulator voltage output Scale 1 mode */
   HAL::ResetControlClock::set_apb1_clock( HAL::ResetControlClock::APB1Clocks::power_management, true );
   HAL::PowerManagement::set_control_register( HAL::PowerManagement::ControlRegister::voltage_output_selection, 0x03 );

   /* setup the clock prescalers */
   HAL::ResetControlClock::configure_ahb_clock( HAL::ResetControlClock::AHBPrescaler::prescaler_none );
   HAL::ResetControlClock::configure_apb1_clock( HAL::ResetControlClock::APBPrescaler::prescaler_4 );
   HAL::ResetControlClock::configure_apb2_clock( HAL::ResetControlClock::APBPrescaler::prescaler_2 );

   /* configure the main phase locked loop */
   HAL::ResetControlClock::configure_main_pll( RCC_PLL_SOURCE, HSE_FREQUENCY, RCC_PLL_M_FACTOR, RCC_PLL_N_FACTOR, RCC_PLL_P_FACTOR, RCC_PLL_Q_FACTOR );

   /* enable the phase locked loop */
   HAL::ResetControlClock::set_control_register( HAL::ResetControlClock::RCCRegister::main_pll_on, 0x01 );

   /* wait for the phase locked loop to stabilize */
   volatile bool pll_ready = false;
   do
   {
      pll_ready = static_cast<bool>( HAL::ResetControlClock::get_control_register( HAL::ResetControlClock::RCCRegister::main_pll_ready ) );
   } while ( pll_ready == false );

   /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
   HAL::Flash::set_access_control_register( HAL::Flash::AccessControlRegister::prefetch_enable, 0x01 );
   HAL::Flash::set_access_control_register( HAL::Flash::AccessControlRegister::instruction_cache_enable, 0x01 );
   HAL::Flash::set_access_control_register( HAL::Flash::AccessControlRegister::data_cache_enable, 0x01 );
   HAL::Flash::set_access_control_register( HAL::Flash::AccessControlRegister::latency, 0x05 );

   /* set the system clock source */
   HAL::ResetControlClock::set_system_clock_source( HAL::ResetControlClock::SystemClockSource::phase_locked_loop );

   /* set the systick interrupt frequency to every ms */
   uint32_t sys_clock = HAL::ResetControlClock::get_clock_speed( HAL::ResetControlClock::Clocks::AHB1 );
   HAL::Interrupt::set_systick_frequency( sys_clock / 1000 );
}
