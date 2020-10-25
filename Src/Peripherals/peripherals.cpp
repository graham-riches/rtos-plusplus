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
#include "hal_power.h"
#include "hal_rcc.h"


/*********************************** Consts ********************************************/
constexpr uint8_t RCC_PLL_M_FACTOR = 8;
constexpr uint16_t RCC_PLL_N_FACTOR = 336;
constexpr uint8_t RCC_PLL_Q_FACTOR = 7;
constexpr HAL::ResetControlClock::PLLOutputPrescaler RCC_PLL_P_FACTOR =
   HAL::ResetControlClock::PLLOutputPrescaler::prescaler_2;
constexpr HAL::ResetControlClock::PLLSource RCC_PLL_SOURCE = HAL::ResetControlClock::PLLSource::high_speed_external;

/************************************ Types ********************************************/

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/

/******************************** Local Variables **************************************/

/****************************** Functions Prototype ************************************/
void PERIPHERAL_systemBoot( void );

/****************************** Functions Definition ***********************************/
/**
* \name     PERIPHERAL_moculdeInitialize
* \brief    setup all of the peripherals
*
* \param    None
* \retval   None
*/
void PERIPHERAL_moduleInitialize( void )
{
   /* boot-up */
   PERIPHERAL_systemBoot( );

   /* Initialize all configured peripherals */
   GPIO_init( );
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
void PERIPHERAL_systemBoot( void )
{
   using namespace HAL;

   /* enable the high speed external clock source */
   ResetControlClock::set_control_register( ResetControlClock::RCCRegister::hse_on, 0x01 );

   /* wait for the high speed clock to stabilize */
   volatile bool hse_ready = false;
   do
   {
      hse_ready = static_cast<bool>( ResetControlClock::get_control_register( ResetControlClock::RCCRegister::hse_ready ) );
   } while ( hse_ready == false );

   /* Select regulator voltage output Scale 1 mode */
   ResetControlClock::set_apb1_clock(ResetControlClock::APB1Clocks::power_management, true );   
   PowerManagement::set_control_register( PowerManagement::ControlRegister::voltage_output_selection, 0x03 );

   /* setup the clock prescalers */
   ResetControlClock::configure_ahb_clock( ResetControlClock::AHBPrescaler::prescaler_none );
   ResetControlClock::configure_apb1_clock( ResetControlClock::APBPrescaler::prescaler_4 );
   ResetControlClock::configure_apb2_clock( ResetControlClock::APBPrescaler::prescaler_2 );

   /* configure the main phase locked loop */
   ResetControlClock::configure_main_pll(
      RCC_PLL_SOURCE, RCC_PLL_M_FACTOR, RCC_PLL_N_FACTOR, RCC_PLL_P_FACTOR, RCC_PLL_Q_FACTOR );

   /* enable the phase locked loop */
   ResetControlClock::set_control_register( ResetControlClock::RCCRegister::main_pll_on, 0x01 );

   /* wait for the phase locked loop to stabilize */
   volatile bool pll_ready = false;
   do
   {
      pll_ready = static_cast<bool>( ResetControlClock::get_control_register( ResetControlClock::RCCRegister::main_pll_ready ) );
   } while ( pll_ready == false );

   /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
   Flash::set_access_control_register( Flash::AccessControlRegister::prefetch_enable, 0x01 );
   Flash::set_access_control_register( Flash::AccessControlRegister::instruction_cache_enable, 0x01 );
   Flash::set_access_control_register( Flash::AccessControlRegister::data_cache_enable, 0x01 );
   Flash::set_access_control_register( Flash::AccessControlRegister::latency, 0x05 );

   /* set the system clock source */
   ResetControlClock::set_system_clock_source( ResetControlClock::SystemClockSource::phase_locked_loop );
}