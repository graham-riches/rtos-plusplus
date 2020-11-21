/*! \file hal_exti.c
*
*  \brief hal external interrupts management for GPIO lines.
*
*
*  \author Graham Riches
*/


/********************************** Includes *******************************************/
#include "hal_exti.h"
#include "hal_rcc.h"

namespace HAL
{
/*********************************** Consts ********************************************/


/************************************ Types ********************************************/


/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/


/******************************** Local Variables **************************************/


/****************************** Functions Prototype ************************************/


/****************************** Functions Definition ***********************************/
/**
 * \brief register an external interrupt
 * 
 * \param port which port to register the interrupt on
 * \param pin the pin
 * \param trigger the trigger for the external interrupt (rising edge, etc)
 * \details SYSCFG has an array of 4 32-bit values which contain the configuration of the 16 EXTI lines.
 *          Each EXTI port configuration is 4-bits at the lower 16-bits of each register in the array.
 *          The GPIO pins are inconveniently bit flags, which makes counting the actual value annoying BUT
 *          there is a solution :)
 *          
 */
void register_external_interrupt( EXTIPort port, Pins pin, EXTITrigger trigger )
{
   /* make sure the syscfg clock is on */
   reset_control_clock.set_apb_clock( APB2Clocks::sys_config, true );

   uint8_t pin_offset = static_cast<uint8_t>( pin );
   uint8_t pin_number = 1;
   while ( pin_offset >>= 1 )
   {      
      pin_number++;
   }

   uint8_t exti_index = pin_number / 4;
   uint8_t exti_offset = ( pin_number - 1 ) % 4;

   /* clear the bits in the register location */
   uint8_t exti_mask = 0xFF;
   SYSCFG->EXTICR[exti_index] &= ~(exti_mask << ( exti_offset * 4 ) );

   /* now write the bits */
   SYSCFG->EXTICR[exti_index] |= (static_cast<uint8_t>( port ) << ( 4 * exti_offset ) );

   /* clear the interrupt mask */
   uint32_t pin_mask = ( 0x01 << ( pin_number - 1 ) );
   EXTI->IMR |= pin_mask;

   /* set the trigger mode */

   switch ( trigger )
   {
      case EXTITrigger::none:
         EXTI->FTSR &= ~pin_mask;
         EXTI->RTSR &= ~pin_mask;
         break;

      case EXTITrigger::rising:
         EXTI->FTSR &= ~pin_mask;
         EXTI->RTSR |= pin_mask;
         break;

      case EXTITrigger::falling:
         EXTI->FTSR |= pin_mask;
         EXTI->RTSR &= ~pin_mask;
         break;

      case EXTITrigger::both:
         EXTI->FTSR |= pin_mask;
         EXTI->RTSR |= pin_mask;
         break;

      default:
         break;
   }
};


/**
 * \brief clear the interrupt pending bit for an EXTI line
 * 
 * \param line the line to clear the interrupt on
 */
void clear_external_interrupt_pending( EXTILine line )
{
   EXTI->PR &= ~( 0x01 << static_cast<uint8_t>( line ) );
}

}
