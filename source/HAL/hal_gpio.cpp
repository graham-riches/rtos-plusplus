/*! \file hal_gpio.cpp
*
*  \brief HAL++ gpio abstraction layer
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "hal_gpio.h"
#include "hal_rcc.h"
#include <map>

namespace HAL
{

/****************************** Constants ***********************************/
constexpr uint8_t total_pins = 16;  //!< total pin count per GPIO bank

/****************************** Macros ***********************************/
#define CLEAR_MODE_REGISTER_MASK      0x03  //!< bitmask to clear the mode register for a pin
#define CLEAR_SPEED_REGISTER_MASK     0x03  //!< bitmask to clear the speed register for a pin
#define CLEAR_PORT_TYPE_REGISTER_MASK 0x01  //!< bitmask to clear the port type register for a pin
#define CLEAR_PULLUP_REGISTER_MASK    0x03  //!< bitmask to clear the pullup/pulldown config register for a pin

/****************************** Local Variables ***********************************/
static std::map<GPIO_TypeDef *, AHB1Clocks> gpio_port_map{
   { GPIOA, AHB1Clocks::gpio_a },
   { GPIOB, AHB1Clocks::gpio_b },
   { GPIOC, AHB1Clocks::gpio_c },
   { GPIOD, AHB1Clocks::gpio_d },
   { GPIOE, AHB1Clocks::gpio_e },
   { GPIOF, AHB1Clocks::gpio_f },
   { GPIOG, AHB1Clocks::gpio_g },
   { GPIOH, AHB1Clocks::gpio_h },
   { GPIOI, AHB1Clocks::gpio_i },
};

/****************************** Functions Definitions ***********************************/

/**
 * \brief Construct a new Pin:: Pin object
 * 
 * \param bank the GPIO bank of the pin
 * \param pin the pin number
 * \param mode the pin mode
 * \param speed pin speed
 * \param pull_mode pull up/down configuration
 * \param output_mode output mode type
 */
PinBase::PinBase( GPIO_TypeDef *bank, Pins pin, PinMode mode, Speed speed, PullMode pull_mode, OutputMode output_mode )
{
   this->bank = bank;
   this->pin = pin;
   this->mode = mode;
   this->speed = speed;
   this->pull_mode = pull_mode;
   this->output_mode = output_mode;

   /* enable the peripheral clock. Note: this must be done before writing to the registers */   
   reset_control_clock.set_ahb_clock( gpio_port_map[bank], true );

   /* find the appropriate bits and set them */
   for ( uint8_t i = 0; i < total_pins; i++ )
   {
      uint32_t test_pin = 0x01 << i;

      if ( static_cast<bool>( test_pin & static_cast<uint32_t>( this->pin ) ) )
      {
         /* set the pin mode */
         this->bank->MODER &= ~( CLEAR_MODE_REGISTER_MASK << ( 2 * i ) );
         this->bank->MODER |= ( static_cast<uint32_t>( this->mode ) << ( 2 * i ) );

         /* set the pin port type (push_pull/open_drain) */
         this->bank->OTYPER &= ~( CLEAR_PORT_TYPE_REGISTER_MASK << i );
         this->bank->OTYPER |= ( static_cast<uint32_t>( this->output_mode ) << i );

         /* set the speed and push pull config only for outputs and alternate function pins */
         if ( ( this->mode == PinMode::output ) || ( this->mode == PinMode::alternate ) )
         {
            /* set the pin speed */
            this->bank->OSPEEDR &= ~( CLEAR_SPEED_REGISTER_MASK << ( 2 * i ) );
            this->bank->OSPEEDR |= ( static_cast<uint32_t>( this->speed ) << ( 2 * i ) );

            /* set the pin push_pull configuration */
            this->bank->PUPDR &= ~( CLEAR_PULLUP_REGISTER_MASK << ( 2 * i ) );
            this->bank->PUPDR |= ( static_cast<uint32_t>( this->pull_mode ) << ( 2 * i ) );
         }
         break;
      }
   }
}

/**
 * \brief Construct a new Output Pin:: Output Pin object
 * 
 * \param bank the pin bank
 * \param pin number
 * \param mode pin mode
 * \param speed pin output speed
 * \param pull_mode pull up or down
 * \param output_mode push/pull
 */
OutputPin::OutputPin( GPIO_TypeDef *bank, Pins pin, PinMode mode, Speed speed, PullMode pull_mode, OutputMode output_mode )
   : PinBase( bank, pin, mode, speed, pull_mode, output_mode )
{ }

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

/**
 * \brief toggle an output pin
 * 
 */
void OutputPin::toggle( void )
{
   uint16_t pin_mask = static_cast<uint16_t>( this->pin );
   this->bank->ODR ^= pin_mask;
}

/**
 * \brief Construct a new Alternate Mode Pin:: Alternate Mode Pin object
 * 
 * \param bank the GPIO banke
 * \param pin pin number
 * \param mode pin mode type
 * \param speed pin speed
 * \param pull_mode pin pull mode
 * \param output_mode output mode (push/pull)
 * \param af_mode the alternate function to set
 */
AlternateModePin::AlternateModePin( GPIO_TypeDef *bank, Pins pin, PinMode mode, Speed speed, PullMode pull_mode, OutputMode output_mode, AlternateMode af_mode )
   : PinBase( bank, pin, mode, speed, pull_mode, output_mode )
{

   this->af_mode = af_mode;

   for ( uint8_t i = 0; i < total_pins; i++ )
   {
      uint32_t test_pin = 0x01 << i;

      if ( static_cast<bool>( test_pin & static_cast<uint32_t>( this->pin ) ) )
      {
         /* select the appropriate register (high/low) from the AFR array */
         uint8_t afr_array_index = ( i < 8 ) ? 0 : 1;

         /* clear the register and set it to the appropriate value */
         uint32_t mask = 0x0F;
         this->bank->AFR[afr_array_index] &= ~( ( mask ) << ( (i%8) * 4 ) );
         this->bank->AFR[afr_array_index] |= ( static_cast<uint8_t>( this->af_mode ) << ( (i%8) * 4 ) );
      }
   }
}

};  // namespace HAL
