/*! \file debug_port.h
*
*  \brief debug_port module functions and variables declarations.
*
*
*  \author Graham Riches
*/

#ifndef __DEBUG_PORT_H__
#define __DEBUG_PORT_H__

/********************************** Includes *******************************************/
#include "common.h"
#include "hal_usart.h"
#include "stm32f4xx.h"
#include <memory>
#include <stdarg.h>
#include <string.h>


/*********************************** Consts ********************************************/

/************************************ Types ********************************************/
/**
 * \brief class definition for a debug port
 * 
 */
class DebugPort : public HAL::USART::USARTInterrupt
{
   private:
   const size_t tx_buffer_size;
   std::unique_ptr<char[]> log_buffer;

   public:
   DebugPort( );
   DebugPort( USART_TypeDef *usart, size_t tx_size, size_t rx_size )
      : HAL::USART::USARTInterrupt( usart, tx_size, rx_size ), tx_buffer_size( tx_size )
   {
      this->log_buffer = std::make_unique<char[]>( tx_size );
   };
   void initialize( void );
   void debug( const char *message, ... );
};

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/
extern DebugPort debug_port;

/****************************** Functions Prototype ************************************/

#endif /* __DEBUG_PORT_H__ */
