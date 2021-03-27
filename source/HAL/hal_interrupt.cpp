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

/*********************************** Consts ********************************************/


/************************************ Types ********************************************/

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/
InterruptManager interrupt_manager;

/******************************** Local Variables **************************************/

/****************************** Functions Prototype ************************************/

/****************************** Functions Definition ***********************************/

/****************************** Vector Table Function Pointer Definitions ***********************************/
extern "C"
{
    /**
    * \brief this single function remaps all vector table interrups to the C++ interrupt handler
    *        which calls the appropriate IRQ which is generally registered to a specific object.
    */
    void IRQHandler(void) {
        HAL::interrupt_manager.default_isr_handler();
    }
}
}