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
#include <cstdarg>
#include <memory>

/*********************************** Consts ********************************************/

/************************************ Types ********************************************/
/**
 * \brief class definition for a debug port
 * 
 */
class DebugPort : public HAL::USART::USARTInterrupt
{
   public:
   DebugPort( );
   DebugPort( USART_TypeDef *usart, size_t tx_size, size_t rx_size )
      : HAL::USART::USARTInterrupt( usart, tx_size, rx_size )
   { };
   void initialize( void );
   void debug( const char *message );
   void info( const char *message );
   void warning( const char *message );
   void error( const char *message );
};

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/
extern DebugPort debug_port;

/****************************** Functions Prototype ************************************/

#endif /* __DEBUG_PORT_H__ */
