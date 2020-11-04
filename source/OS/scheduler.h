/*! \file scheduler.h
*
*  \brief simple cooperative scheduler for multi-tasking
*
*
*  \author Graham Riches
*/

#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

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

/****************************** Functions Prototype ************************************/
void enter_kernel( void );

};  // namespace OS
#ifdef __cplusplus
}
#endif

#endif /* __SCHEDULER_H__ */
