/*! \file kernel.h
*
*  \brief kernel module functions and variables declarations.
*
*
*  \author Graham Riches
*/

#ifndef __KERNEL_H__
#define __KERNEL_H__

/********************************** Includes *******************************************/
#include "common.h"

namespace OS
{

/****************************** Functions Declarations ************************************/
void enter_kernel(void);
void set_systick_frequency(uint32_t ticks);

};  // namespace OS

#endif /* __KERNEL_H__ */
