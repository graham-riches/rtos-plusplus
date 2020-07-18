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
#include "utilities.h"

/*********************************** Consts ********************************************/


/************************************ Types ********************************************/


/*********************************** Macros ********************************************/
#define PARAMETER_NOT_USED(X) (void)(X)
#define CHECK_ENUM_INPUT(X, MAX) if ( X >= MAX ){ return; }

#ifndef NULL
#define NULL ((void *)0)
#endif

/******************************* Global Variables **************************************/


/****************************** Functions Prototype ************************************/



#ifdef __cplusplus
}
#endif

#endif /* __SPI_H__ */
