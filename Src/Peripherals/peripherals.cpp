/*! \file peripherals.cpp
*
*  \brief HAL setup and abstraction for all peripherals
*
*
*  \author Graham Riches
*/


/********************************** Includes *******************************************/
#include "peripherals.h"
#include "rcc.h"
#include "gpio.h"
#include "hal_rcc.h"

/*********************************** Consts ********************************************/
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
	GPIO_init();

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

   /* TODO: power management module here */
   /* Select regulator voltage output Scale 1 mode */
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_VOS;

   /* setup the clock prescalers */
   ResetControlClock::configure_ahb_clock( ResetControlClock::AHBPrescaler::prescaler_none );
   ResetControlClock::configure_apb1_clock( ResetControlClock::APBPrescaler::prescaler_4 );
   ResetControlClock::configure_apb2_clock( ResetControlClock::APBPrescaler::prescaler_2 );
      
   /* configure the main phase locked loop */
   ResetControlClock::configure_main_pll( RCC_PLL_SOURCE, RCC_PLL_M_FACTOR, RCC_PLL_N_FACTOR, RCC_PLL_P_FACTOR, RCC_PLL_Q_FACTOR );

   /* enable the phase locked loop */
   ResetControlClock::set_control_register( ResetControlClock::RCCRegister::main_pll_on, 0x01 );

   /* wait for the phase locked loop to stabilize */
   volatile bool pll_ready = false;
   do
   {
      pll_ready = static_cast<bool>( ResetControlClock::get_control_register( ResetControlClock::RCCRegister::main_pll_ready ) );
   } while ( pll_ready == false );

   /* TODO: flash module here */
   /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
    FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_LATENCY_5WS;

   /* set the system clock source */
   ResetControlClock::set_system_clock_source( ResetControlClock::SystemClockSource::phase_locked_loop );
}