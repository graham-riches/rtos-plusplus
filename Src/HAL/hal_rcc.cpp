/*! \file hal_rcc.cpp
*
*  \brief HAL++ reset and control clock abstration layer
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "hal_rcc.h"

namespace HAL
{
namespace ResetControlClock
{
/************************************ Function Definitions ********************************************/
/**
 * \brief get a control register value
 * 
 * \param register the register to get
 * \retval register value
 * \note register sizes are not consistent, which makes this a tad annoying :(
 */
uint8_t get_control_register( RCCRegister reg )
{
   uint32_t mask = 0;

   switch ( reg )
   {
      /* handle all the single bit registers */
      case RCCRegister::hsi_on:
      case RCCRegister::hsi_ready:      
      case RCCRegister::hse_on:
      case RCCRegister::hse_ready:
      case RCCRegister::hse_bypass:
      case RCCRegister::clock_security:
      case RCCRegister::main_pll_on:
      case RCCRegister::main_pll_ready:
      case RCCRegister::i2s_pll_on:
      case RCCRegister::i2s_pll_ready:
         mask = static_cast<uint32_t>( ( 0x01u << static_cast<uint32_t>( reg ) ));         
         break;

      case RCCRegister::hsi_trim:
         mask = static_cast<uint32_t>( ( 0xFFu << static_cast<uint32_t>( reg ) ) );         
         break;

      case RCCRegister::hsi_cal:
         mask = static_cast<uint32_t>( ( 0x1Fu << static_cast<uint32_t>( reg ) ) );
         break;

      default:         
         break;
   }
   
   uint8_t value = static_cast<uint8_t>( (RCC->CR & mask) >> static_cast<uint8_t>( reg ) );
   return value;
}


/**
 * \brief Set the control register object
 * 
 * \param register the register to set
 * \param value to set
 */
void set_control_register( RCCRegister reg, uint8_t value )
{
   RCC->CR |= ( value << static_cast<uint8_t>( reg ) );
}


/**
 * \brief setup the main phase-locked loop
 * 
 * \param pll_config the configuration
 */
void configure_main_pll( PLLSource clock_source, uint8_t pll_m, uint16_t pll_n, PLLOutputPrescaler pll_p, uint8_t pll_q )
{
   /* set the phase locked loop clock source */
   RCC->PLLCFGR |= ( static_cast<uint8_t>( clock_source ) << static_cast<uint8_t>( PLLRegister::pll_source ) );

   /* set the main division factor for the system clock: PLL_P */
   RCC->PLLCFGR |= ( static_cast<uint8_t>( pll_p ) << static_cast<uint8_t>( PLLRegister::pll_p ) );

   /* set the main multiplication factor for the system clock: PLL_N */
   RCC->PLLCFGR |= ( pll_n << static_cast<uint8_t>( PLLRegister::pll_n ) );

   /* set the division factor for the entry to the VCO: PLL_M */
   RCC->PLLCFGR |= ( pll_m << static_cast<uint8_t>( PLLRegister::pll_m ) );

   /* set the output for the 48 MHz clock source: PLL_Q */
   RCC->PLLCFGR |= ( pll_q << static_cast<uint8_t>( PLLRegister::pll_q ) );

}


/**
 * \brief Set the system clock
 * 
 * \param source 
 */
void set_system_clock_source( SystemClockSource source )
{
   /* clear the current clock source registers */
   uint32_t clock_source_mask = 0b11;

   RCC->CFGR &= ~( clock_source_mask );
   RCC->CFGR |= ( static_cast<uint8_t>( source ) << static_cast<uint8_t>( ConfigurationRegister::system_clock_source ) );

   /* wait until the clock source status register matches the selected clock */
   while ( (RCC->CFGR & clock_source_mask) != static_cast<uint32_t>( source) )
   {

   }
}


/**
 * \brief configure the ahb clock prescaler
 * 
 * \param prescaler 
 */
void configure_ahb_clock( AHBPrescaler prescaler )
{
   RCC->CFGR |= ( static_cast<uint8_t>( prescaler ) << static_cast<uint8_t>(ConfigurationRegister::ahb_prescaler) );
}


/**
 * \brief configure the apb2 bus prescaler
 * 
 * \param prescaler 
 */
void configure_apb2_clock( APBPrescaler prescaler )
{
   RCC->CFGR |= ( static_cast<uint8_t>( prescaler ) << static_cast<uint8_t>(ConfigurationRegister::apb2_prescaler) );
}


/**
 * \brief configure the apb1 clock prescaler
 * 
 * \param prescaler 
 */
void configure_apb1_clock( APBPrescaler prescaler )
{
   RCC->CFGR |= ( static_cast<uint8_t>( prescaler ) << static_cast<uint8_t>(ConfigurationRegister::apb1_prescaler) );
}



};  // namespace ResetControlClock
};  // namespace HAL