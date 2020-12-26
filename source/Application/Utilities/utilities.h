/*! \file utilities.h
*
*  \brief utilities module functions and variables declarations.
*
*
*  \author Graham Riches
*/

#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#ifdef __cplusplus
extern "C"
{
#endif
/********************************** Includes *******************************************/
#include "common.h"

/*********************************** Consts ********************************************/
#define UTILITIES_MAX_STRSPLIT_LENGTH 512

/************************************ Types ********************************************/

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/

/****************************** Functions Prototype ************************************/
int32_t UTILITIES_strSplit(char* inputString, char delimiter, int maxStrings, char* argv[]);

#ifdef __cplusplus
}
#endif

#endif /* __UTILITIES_H__ */
