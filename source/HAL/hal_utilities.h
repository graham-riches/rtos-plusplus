/**
 * \file hal_utilities.h
 * \author Graham Riches
 * \brief Utilities for Hardware Abstraction Layer
 * \version 0.1
 * \date 2022-03-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include <cstdint>
#include <type_traits>

namespace HAL {

/**
 * \brief Set a set of register bits
 *  
 * \param reg The register to set
 * \param bits Bits to set
 */
template <typename T1, typename T2>
void set_bits(volatile T1& reg, T2 bits) {
    static_assert(std::is_integral_v<T1>, "Must be an integral type");
    static_assert(std::is_integral_v<T2>, "Must be an integral type");
    reg = reg | static_cast<uint32_t>(bits);
}

/**
 * \brief Clear a set of bits
 * 
 * @tparam T Integral type of bits
 * \param reg Register to clear bits on
 * \param bits The bits to clear
 */
template <typename T1, typename T2>
void clear_bits(volatile T1& reg, T2 bits) {
    //!< TODO: should be able to use an integral concept here instead of static_assert
    static_assert(std::is_integral_v<T1>, "Must be an integral type");
    static_assert(std::is_integral_v<T2>, "Must be an integral type");
    reg = reg & ~static_cast<uint32_t>(bits);
}


/**
 * \brief Checks if a bit is set in a register
 * 
 * @tparam T Register type
 * \param reg The register to check
 * \param mask Bitmask to check against
 * \return bool True if set
 */
template <typename RegisterType, typename BitType>
bool is_bit_set(RegisterType&& reg, BitType mask) {
    static_assert(std::is_integral_v<RegisterType>, "Must be an integral type");
    static_assert(std::is_integral_v<BitType>, "Must be an integral type");
    return (static_cast<BitType>(reg) & mask) == mask;
}


};