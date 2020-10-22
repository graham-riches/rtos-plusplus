/*! \file gpio_gpio.cpp
*
*  \brief HAL++ gpio abstraction layer
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "hal_gpio.h"
#include <array>

namespace HAL
{
namespace GPIO
{
/****************************** Macros ***********************************/
#define CLEAR_MODE_REGISTER_MASK      0x03 //!< bitmask to clear the mode register for a pin
#define CLEAR_SPEED_REGISTER_MASK     0x03 //!< bitmask to clear the speed register for a pin
#define CLEAR_PORT_TYPE_REGISTER_MASK 0x01 //!< bitmask to clear the port type register for a pin
#define CLEAR_PULLUP_REGISTER_MASK    0x03 //!< bitmask to clear the pullup/pulldown config register for a pin

/****************************** Functions Definitions ***********************************/
/**
 * \brief initialize a GPIO pin
 * 
 * \param GPIOx reference to the internal GPIO registers for the peripheral
 * \param configuration configuration structure 
 */
void initialize_pin( GPIO_TypeDef *GPIOx, PinConfiguration configuration )
{
   for ( uint8_t i = 0; i < static_cast<int>( Pins::all ); i++ )
   {
      Pins pin = static_cast<Pins>( 0x01 << i );
      if ( static_cast<bool>( pin & configuration.pins ) )
      {
         /* set the pin mode */
         GPIOx->MODER &= ~( CLEAR_MODE_REGISTER_MASK << ( 2 * i ) );
         GPIOx->MODER |= ( static_cast<int>( configuration.mode ) << ( 2 * i ) );

         /* set the pin port type (push_pull/open_drain) */
         GPIOx->OTYPER &= ~( CLEAR_PORT_TYPE_REGISTER_MASK << i );
         GPIOx->OTYPER |= ( static_cast<int>( configuration.output_mode ) << i );

         /* set the speed and push pull config only for outputs and alternate function pins */
         if ( ( configuration.mode == GPIO::PinMode::output ) || ( configuration.mode == GPIO::PinMode::alternate ) )
         {
            /* set the pin speed */
            GPIOx->OSPEEDR &= ~( CLEAR_SPEED_REGISTER_MASK << ( 2 * i ) );
            GPIOx->OSPEEDR |= ( static_cast<int>( configuration.speed ) << ( 2 * i ) );

            /* set the pin push_pull configuration */
            GPIOx->PUPDR &= ~( CLEAR_PULLUP_REGISTER_MASK << ( 2 * i ) );
            GPIOx->PUPDR |= ( static_cast<int>( configuration.pull_mode ) << ( 2 * i ) );
         }
      }
   }
}

};  // namespace GPIO
};  // namespace HAL
