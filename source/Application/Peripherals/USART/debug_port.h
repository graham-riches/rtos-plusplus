/*! \file debug_port.h
*
*  \brief debug_port module functions and variables declarations.
*
*
*  \author Graham Riches
*/

#pragma once

/********************************** Includes *******************************************/
#include "common.h"
#include "hal_gpio.h"
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
class DebugPort : public HAL::USARTInterrupt {
  private:
    std::unique_ptr<char[]> print_buffer;
    size_t print_buffer_size;
    void log_message(const char* message, const char* tag, va_list args);

  public:
    DebugPort();
    DebugPort(USART_TypeDef* usart);
    void initialize(void);
    void debug(const char* message, ...);
    void info(const char* message, ...);
    void warning(const char* message, ...);
    void error(const char* message, ...);
};

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/
extern DebugPort debug_port;
