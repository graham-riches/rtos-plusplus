/*! \file hal_interrupt.cpp
*
*  \brief abstract base class for interrupt managemente.
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "hal_interrupt.h"
#include "stm32f4xx.h"

namespace HAL
{
/******************************* Global Variables **************************************/
InterruptManager interrupt_manager;

}