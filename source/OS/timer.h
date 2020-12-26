/*! \file timer.h
*
*  \brief timer module functions and variables declarations.
*
*
*  \author Graham Riches
*/

#ifndef __TIMER_H__
#define __TIMER_H__

#ifdef __cplusplus
extern "C"
{
#endif
/********************************** Includes *******************************************/
#include "common.h"

namespace OS
{
/*********************************** Consts ********************************************/

/************************************ Types ********************************************/

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/
extern volatile uint32_t system_ticks;

/****************************** Functions Prototype ************************************/
void delay_ms(uint32_t milliseconds);

};  // namespace OS
#ifdef __cplusplus
}
#endif

#endif /* __TIMER_H__ */
