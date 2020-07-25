/*! \file common.h
*
*  \brief common module includes
*
*
*  \author Graham Riches
*/

#ifndef __COMMON_H__
#define __COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif
/********************************** Includes *******************************************/
#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"

/*********************************** Consts ********************************************/


/************************************ Types ********************************************/


/*********************************** Macros ********************************************/
#define PARAMETER_NOT_USED(X) (void)(X)

#ifndef NULL
    #define NULL ((void *)0)
#endif

/* NOTE: these macros must exist in pairs! */
#define DISABLE_INTERRUPTS() uint32_t prim = __get_PRIMASK(); __disable_irq();
#define ENABLE_INTERRUPTS() if(!prim){__enable_irq();}

/* setup arm math */
#ifndef ARM_MATH_CM4
    #define ARM_MATH_CM4
#endif


/******************************* Global Variables **************************************/


/****************************** Functions Prototype ************************************/



#ifdef __cplusplus
}
#endif

#endif /* __COMMON_H */
