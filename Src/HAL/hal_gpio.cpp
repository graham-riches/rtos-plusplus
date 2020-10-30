/*! \file hal_gpio.cpp
*
*  \brief HAL++ gpio abstraction layer
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "hal_gpio.h"

namespace HAL
{
namespace GPIO
{

/****************************** Macros ***********************************/
#define CLEAR_MODE_REGISTER_MASK      0x03  //!< bitmask to clear the mode register for a pin
#define CLEAR_SPEED_REGISTER_MASK     0x03  //!< bitmask to clear the speed register for a pin
#define CLEAR_PORT_TYPE_REGISTER_MASK 0x01  //!< bitmask to clear the port type register for a pin
#define CLEAR_PULLUP_REGISTER_MASK    0x03  //!< bitmask to clear the pullup/pulldown config register for a pin

/****************************** Local Function Prototypes ***********************************/


/****************************** Functions Definitions ***********************************/
/**
 * \brief initialize a GPIO pin
 * 
 * \param bank reference to the internal GPIO registers for the peripheral
 * \param configuration configuration structure 
 */
void initialize_pin(
   GPIO_TypeDef *bank, Pins pin, PinMode mode, Speed speed, PullMode pull_mode, OutputMode output_mode )
{
   /* find the appropriate bits and set them */
   /* TODO fix magic number  */
   for ( uint8_t i = 0; i < 16; i++ )
   {
      uint32_t test_pin = 0x01 << i;

      if ( static_cast<bool>( test_pin & static_cast<uint32_t>( pin ) ) )
      {
         /* set the pin mode */
         bank->MODER &= ~( CLEAR_MODE_REGISTER_MASK << ( 2 * i ) );
         bank->MODER |= ( static_cast<uint32_t>( mode ) << ( 2 * i ) );

         /* set the pin port type (push_pull/open_drain) */
         bank->OTYPER &= ~( CLEAR_PORT_TYPE_REGISTER_MASK << i );
         bank->OTYPER |= ( static_cast<uint32_t>( output_mode ) << i );

         /* set the speed and push pull config only for outputs and alternate function pins */
         if ( ( mode == GPIO::PinMode::output ) || ( mode == GPIO::PinMode::alternate ) )
         {
            /* set the pin speed */
            bank->OSPEEDR &= ~( CLEAR_SPEED_REGISTER_MASK << ( 2 * i ) );
            bank->OSPEEDR |= ( static_cast<uint32_t>( speed ) << ( 2 * i ) );

            /* set the pin push_pull configuration */
            bank->PUPDR &= ~( CLEAR_PULLUP_REGISTER_MASK << ( 2 * i ) );
            bank->PUPDR |= ( static_cast<uint32_t>( pull_mode ) << ( 2 * i ) );
         }
      }
   }
}

/**
 * \brief Set the alternate mode for a pin
 * 
 * \param bank which GPIO port
 * \param pins to set
 * \param alternate what alternate mode
 */
void set_alternate_mode( GPIO_TypeDef *bank, Pins pins, AlternateMode alternate )
{
   for ( uint8_t i = 0; i < 16; i++ )
   {
      uint32_t test_pin = 0x01 << i;

      if ( static_cast<bool>( test_pin & static_cast<uint32_t>( pins ) ) )
      {
         /* select the appropriate register (high/low) from the AFR array */
         uint8_t afr_array_index = ( test_pin <= 8 ) ? 0 : 1;
         
         /* clear the register and set it to the appropriate value */
         uint32_t mask = 0x0F;
         bank->AFR[afr_array_index] &= ~( ( mask ) << ( static_cast<uint8_t>(alternate) * 4 ) );
         bank->AFR[afr_array_index] |= ( static_cast<uint8_t>( alternate ) << ( i * 4 ) );
      }
   }
}

/**
 * \brief initialize a gpio output pin object
 * 
 * \param bank the GPIO bank of the pin i.e. GPIOA, GPIOB, etc.
 * \param pin the pin number 
 * \param speed output speed
 * \param pull_mode pullup, pulldown, etc.
 * \param output_mode open drain, etc.
 */
void OutputPin::initialize( GPIO_TypeDef *bank, Pins pin, Speed speed, PullMode pull_mode, OutputMode output_mode )
{
   this->bank = bank;
   this->pin = pin;

   /* initialize the pin as an output */
   initialize_pin( bank, pin, PinMode::output, speed, pull_mode, output_mode );
}

/**
 * \brief set an output pins state
 * 
 * \param high true to set the pin high
 */
void OutputPin::set( bool high )
{
   uint16_t pin_mask = static_cast<uint16_t>( this->pin );

   if ( high )
   {
      this->bank->BSRRL |= pin_mask;
   }
   else
   {
      this->bank->BSRRH |= pin_mask;
   }
}




};  // namespace GPIO
};  // namespace HAL
