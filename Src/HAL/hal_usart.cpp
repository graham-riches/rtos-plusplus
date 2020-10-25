/*! \file hal_usart.c
*
*  \brief HAL++ implementation of the STM32 USART peripherals.
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "hal_usart.h"

namespace HAL
{
namespace USART
{

/*********************************** Consts ********************************************/

/************************************ Types ********************************************/

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/

/******************************** Local Variables **************************************/

/****************************** Functions Prototype ************************************/

/****************************** Function Definitions ***********************************/
/**
 * \brief read a value from the status register
 * 
 * \param usart which usart peripheral to read
 * \param reg the register 
 * \retval register value
 */
bool read_status_register( USART_TypeDef *usart, StatusRegister reg )
{
   return static_cast<bool>( usart->SR & ( 0x01u << static_cast<uint8_t>( reg ) ) );
}

/**
 * \brief write a value into a usart peripherals CR1
 * 
 * \param usart the usart to write to
 * \param reg the register
 * \param value value
 */
void write_control_register( USART_TypeDef *usart, ControlRegister1 reg, uint8_t value )
{
   /* clear the register */
   usart->CR1 &= ~( 0x01u << static_cast<uint8_t>( reg ) );

   /* write the value */
   usart->CR1 |= ( ( value & 0x01 ) << static_cast<uint8_t>( reg ) );
}

};  // namespace USART
};  // namespace HAL
