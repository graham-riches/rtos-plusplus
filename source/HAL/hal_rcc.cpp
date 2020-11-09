/*! \file hal_rcc.cpp
*
*  \brief HAL++ reset and control clock abstration layer
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "hal_rcc.h"
#include <map>

namespace HAL
{

/************************************ Local Function Definitions ********************************************/


/************************************ Global Variables ********************************************/
ResetControlClock reset_control_clock( RCC ); //!< single instance of the RCC peripheral object created with a pointer to the RCC address


/************************************ Local Variables ********************************************/
/* map ahb prescaler values to integers */
static std::map<AHBPrescaler, uint32_t> ahb_scaler_map
{
   {AHBPrescaler::prescaler_none, 1}, {AHBPrescaler::prescaler_2, 2}, {AHBPrescaler::prescaler_4, 4},
   {AHBPrescaler::prescaler_8, 8}, {AHBPrescaler::prescaler_16, 16}, {AHBPrescaler::prescaler_64, 64},
   {AHBPrescaler::prescaler_128, 128}, {AHBPrescaler::prescaler_256, 256}, {AHBPrescaler::prescaler_512, 512},
};

/* map apb prescalers to integers */
static std::map<APBPrescaler, uint32_t> apb_scaler_map
{
   {APBPrescaler::prescaler_none, 1}, {APBPrescaler::prescaler_2, 2}, {APBPrescaler::prescaler_4, 4},
   {APBPrescaler::prescaler_8, 8}, {APBPrescaler::prescaler_16, 16},
};

/************************************ Function Definitions ********************************************/
/**
 * \brief Construct a new Reset Control Clock:: Reset Control Clock object
 * \note this is intended to be a single instance, but there is no restriction
 *       placed on this via singleton or whatnot.
 * 
 * \param rcc_peripheral_address the address of the rcc
 */
ResetControlClock::ResetControlClock( RCC_TypeDef *rcc_peripheral_address )
{
   this->rcc = rcc_peripheral_address;
}


/**
 * \brief get a control register value
 * 
 * \param register the register to get
 * \retval register value
 * \note register sizes are not consistent, which makes this a tad annoying :(
 */
uint8_t ResetControlClock::get_control_register( RCCRegister reg )
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
         mask = static_cast<uint32_t>( ( 0x01u << static_cast<uint32_t>( reg ) ) );
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

   uint8_t value = static_cast<uint8_t>( ( this->rcc->CR & mask ) >> static_cast<uint8_t>( reg ) );
   return value;
}

/**
 * \brief Set the control register
 * 
 * \param register the register to set
 * \param value to set
 */
void ResetControlClock::set_control_register( RCCRegister reg, uint8_t value )
{
   this->rcc->CR |= ( value << static_cast<uint8_t>( reg ) );
}

/**
 * \brief setup the main PLL
 * 
 * \param clock_source clock source (HSI, HSE)
 * \param oscillator_speed how fast the oscillator is running
 * \param pll_m m prescaler
 * \param pll_n n prescaler
 * \param pll_p p prescaler
 * \param pll_q q prescaler
 */
void ResetControlClock::configure_main_pll(
   PLLSource clock_source, uint32_t oscillator_speed, uint8_t pll_m, uint16_t pll_n, PLLOutputPrescaler pll_p, uint8_t pll_q )
{
   /* clear the register */
   this->rcc->PLLCFGR = 0;

   /* set the phase locked loop clock source */
   this->rcc->PLLCFGR |= ( static_cast<uint8_t>( clock_source ) << static_cast<uint8_t>( PLLRegister::pll_source ) );

   /* set the main division factor for the system clock: PLL_P */
   this->rcc->PLLCFGR |= ( static_cast<uint8_t>( pll_p ) << static_cast<uint8_t>( PLLRegister::pll_p ) );

   /* set the main multiplication factor for the system clock: PLL_N */
   this->rcc->PLLCFGR |= ( pll_n << static_cast<uint8_t>( PLLRegister::pll_n ) );

   /* set the division factor for the entry to the VCO: PLL_M */
   this->rcc->PLLCFGR |= ( pll_m << static_cast<uint8_t>( PLLRegister::pll_m ) );

   /* set the output for the 48 MHz clock source: PLL_Q */
   this->rcc->PLLCFGR |= ( pll_q << static_cast<uint8_t>( PLLRegister::pll_q ) );

   /* set the system clock speed */
   uint8_t pll_p_scaler = ( static_cast<uint8_t>( pll_p ) + 1 ) * 2;
   this->clock_configuration.system_clock = ( oscillator_speed * pll_n ) / ( pll_m * pll_p_scaler );
   this->save_clock_configuration();
}

/**
 * \brief Set the system clock
 * 
 * \param source the clock source for the system
 */
void ResetControlClock::set_system_clock_source( SystemClockSource source )
{
   /* clear the current clock source registers */
   uint32_t clock_source_mask = 0b11;

   this->rcc->CFGR &= ~( clock_source_mask );
   this->rcc->CFGR |=
      ( static_cast<uint8_t>( source ) << static_cast<uint8_t>( ConfigurationRegister::system_clock_source ) );

   /* wait until the clock source status register matches the selected clock */
   while ( ( this->rcc->CFGR & clock_source_mask ) != static_cast<uint32_t>( source ) )
   { }
}

/**
 * \brief configure the ahb clock prescaler
 * 
 * \param prescaler 
 */
void ResetControlClock::configure_ahb_clock( AHBPrescaler prescaler )
{
   this->rcc->CFGR |= ( static_cast<uint8_t>( prescaler ) << static_cast<uint8_t>( ConfigurationRegister::ahb_prescaler ) );

   /* store the clock configuration for the AHB clock */
   this->clock_configuration.ahb_scaler = ahb_scaler_map[prescaler];
   this->save_clock_configuration();
}

/**
 * \brief configure the apb2 bus prescaler
 * 
 * \param prescaler 
 */
void ResetControlClock::configure_apb2_clock( APBPrescaler prescaler )
{
   this->rcc->CFGR |= ( static_cast<uint8_t>( prescaler ) << static_cast<uint8_t>( ConfigurationRegister::apb2_prescaler ) );

   /* store the clock configuration for the APB1 clock */
   this->clock_configuration.apb2_scaler = apb_scaler_map[prescaler];
   this->save_clock_configuration();
}

/**
 * \brief configure the apb1 clock prescaler
 * 
 * \param prescaler 
 */
void ResetControlClock::configure_apb1_clock( APBPrescaler prescaler )
{
   this->rcc->CFGR |= ( static_cast<uint8_t>( prescaler ) << static_cast<uint8_t>( ConfigurationRegister::apb1_prescaler ) );

   /* store the clock configuration for the APB1 clock */
   this->clock_configuration.apb1_scaler = apb_scaler_map[prescaler];
   this->save_clock_configuration();
}

/**
 * \brief enable or disable a clock source on the ahb1 bus
 * 
 * \param clock the clock to enable/disable
 * \param enable state
 */
void ResetControlClock::set_ahb_clock( AHB1Clocks clock, bool enable )
{
   if ( enable )
   {
      this->rcc->AHB1ENR |= ( static_cast<uint8_t>( enable ) << static_cast<uint8_t>( clock ) );
   }
   else
   {
      this->rcc->AHB1ENR &= ~( static_cast<uint8_t>( enable ) << static_cast<uint8_t>( clock ) );
   }
}

/**
 * \brief enable or disable a clock source on the ahb2 bus
 * 
 * \param clock the clock to enable/disable
 * \param enable state
 */
void ResetControlClock::set_ahb_clock( AHB2Clocks clock, bool enable )
{
   if ( enable )
   {
      this->rcc->AHB2ENR |= ( static_cast<uint8_t>( enable ) << static_cast<uint8_t>( clock ) );
   }
   else
   {
      this->rcc->AHB2ENR &= ~( static_cast<uint8_t>( enable ) << static_cast<uint8_t>( clock ) );
   }
}

/**
 * \brief enable or disable a clock source on the ahb3 bus
 * 
 * \param clock the clock to enable/disable
 * \param enable state
 */
void ResetControlClock::set_ahb_clock( AHB3Clocks clock, bool enable )
{
   if ( enable )
   {
      this->rcc->AHB3ENR |= ( static_cast<uint8_t>( enable ) << static_cast<uint8_t>( clock ) );
   }
   else
   {
      this->rcc->AHB3ENR &= ~( static_cast<uint8_t>( enable ) << static_cast<uint8_t>( clock ) );
   }
}

/**
 * \brief enable or disable a clock source on the apb1 bus
 * 
 * \param clock the clock to enable/disable
 * \param enable state
 */
void ResetControlClock::set_apb_clock( APB1Clocks clock, bool enable )
{
   if ( enable )
   {
      this->rcc->APB1ENR |= ( static_cast<uint8_t>( enable ) << static_cast<uint8_t>( clock ) );
   }
   else
   {
      this->rcc->APB1ENR &= ~( static_cast<uint8_t>( enable ) << static_cast<uint8_t>( clock ) );
   }
}

/**
 * \brief enable or disable a clock source on the apb2 bus
 * 
 * \param clock the clock to enable/disable
 * \param enable state
 */
void ResetControlClock::set_apb_clock( APB2Clocks clock, bool enable )
{
   if ( enable )
   {
      this->rcc->APB2ENR |= ( static_cast<uint8_t>( enable ) << static_cast<uint8_t>( clock ) );
   }
   else
   {
      this->rcc->APB2ENR &= ~( static_cast<uint8_t>( enable ) << static_cast<uint8_t>( clock ) );
   }
}

/**
 * \brief Get the clock speed for a specific bus
 * 
 * \param clock the clock bus
 * \retval uint32_t speed in Hz
 */
uint32_t ResetControlClock::get_clock_speed( Clocks clock ) 
{
   uint32_t clock_speed = this->clock_configuration.system_clock;
   switch ( clock )
   {
      /* intentional fallthrough */
      case Clocks::AHB1:
      case Clocks::AHB2:
      case Clocks::AHB3:
         clock_speed = this->clock_configuration.ahb;
         break;
      
      case Clocks::APB1:
         clock_speed = this->clock_configuration.apb1;
         break;
      
      case Clocks::APB2:
         clock_speed = this->clock_configuration.apb2;
         break;
      
      default:
         break;
   }
   return clock_speed;
}

/**
 * \brief save the current clock speed
 */
void ResetControlClock::save_clock_configuration( void )
{
   this->clock_configuration.ahb = this->clock_configuration.system_clock / this->clock_configuration.ahb_scaler;
   this->clock_configuration.apb1 = this->clock_configuration.system_clock / this->clock_configuration.apb1_scaler;
   this->clock_configuration.apb2 = this->clock_configuration.system_clock / this->clock_configuration.apb2_scaler;
}


};  // namespace HAL