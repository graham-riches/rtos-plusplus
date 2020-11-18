/*! \file hal_exti.h
*
*  \brief external interrupts management for GPIO lines
*
*
*  \author Graham Riches
*/

#ifndef __HAL_EXTI_H__
#define __HAL_EXTI_H__


/********************************** Includes *******************************************/
#include "common.h"
#include "stm32f4xx.h"
#include "hal_gpio.h"

namespace HAL
{

/*********************************** Consts ********************************************/


/************************************ Types ********************************************/
/**
 * \brief enumeration for selecting exti source
 * \note all pin numbers with the same number (i.e. PA0, PB0) share a single
 *       interrupt line to the processor
 */
enum class EXTIPort : unsigned
{
   gpio_port_a = 0,
   gpio_port_b,
   gpio_port_c,
   gpio_port_d,
   gpio_port_e,
   gpio_port_f,
   gpio_port_g,
   gpio_port_h,
   gpio_port_i,
};

/**
 * \brief enumeration of exti trigger types
 */
enum class EXTITrigger : unsigned
{
   none = 0,
   rising,
   falling,
   both,
};

/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/


/****************************** Functions Prototype ************************************/
void register_external_interrupt( EXTIPort port, Pins pin, EXTITrigger trigger );

};

#endif /* __HAL_EXTI_H__ */
