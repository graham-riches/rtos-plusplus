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


/*********************************** Consts ********************************************/


/************************************ Types ********************************************/


/*********************************** Macros ********************************************/
#define PARAMETER_NOT_USED(X) (void)(X)

#ifndef NULL
    #define NULL ((void *)0)
#endif

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
