/*! \file hal_flash.hpp
*
*  \brief hal_flash module functions and variables declarations.
*
*
*  \author Graham Riches
*/

#pragma once

/********************************** Includes *******************************************/
#include "stm32f4xx.h"

namespace HAL::flash
{
/************************************ Types ********************************************/
/**
 * \brief bit offsets for the flash peripheral access control register
 * 
 */
enum class access_control_register : unsigned {
    latency = 0,
    prefetch_enable = 8,
    instruction_cache_enable = 9,
    data_cache_enable = 10,
    instruction_cache_reset = 11,
    data_cache_reset = 12
};

/****************************** Function Declarations ************************************/
/**
 * \brief Set a bit in the flash access control register
 * 
 * \param reg The register to set the bit 
 * \param bit Bit to set
 * \param value Bit set high or low
 */
void set_access_control_register_bit(access_control_register bit, uint8_t value);

};  // namespace HAL::flash
