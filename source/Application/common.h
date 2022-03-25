/**
 * \file common.h
 * \author Graham Riches
 * \brief Common includes/definitions
 * \version 0.1
 * \date 2022-03-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

/********************************** Includes *******************************************/
#include <cstdint>

/*********************************** Macros ********************************************/
#define PARAMETER_NOT_USED(X) (void)(X)

#ifndef NULL
#    define NULL ((void*)0)
#endif

#ifndef ARM_MATH_CM4
#    define ARM_MATH_CM4
#endif

#ifndef __STATIC_INLINE
#    define __STATIC_INLINE static inline
#endif

#ifndef __GNUC__
#    define __GNUC__
#endif
