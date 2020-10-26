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

/**
 * \brief Set the baudrate for a usart peripheral
 * 
 * \param usart the usart peripheral
 * \param base_clock_rate the base clock rate for the peripheral
 * \param baudrate baudrate to set
 * \todo remove base clock rate and get that direct from HAL_rcc
 * \todo account for oversampling mode
 */
void set_baudrate( USART_TypeDef *usart, uint32_t base_clock_rate, uint32_t baudrate );
{
   uint32_t usart_div = ( 100 * base_clock_rate ) / ( 16 * baudrate );

   /* calculate the integer component */
   uint32_t integer_component = usart_div / 100;

   /* calculate the fractional component */
   uint32_t fractional_temp = usart_div - ( 100 * integer_component );
   
   /* round the fractional component */
   uint32_t fractional_component = ((fractional_temp * 16) + 50 ) / 100 );

   /* build out the register value */
   uint32_t register_value = (integer_component << 4) | ( fractional_component & 0x0F );

   /* write it to the register */
   usart->BRR = register_value;
}

};  // namespace USART
};  // namespace HAL
