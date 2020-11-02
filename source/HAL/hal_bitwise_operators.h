/*! \file hal_bitwise_operators.h
*
*  \brief a set of template functions for creating bitwise operators for scoped enum classes
*
*
*  \author Graham Riches
*/

#ifndef __HAL_BITWISE_OPS_H__
#define __HAL_BITWISE_OPS_H__

/********************************** Includes *******************************************/
#include <stdint.h>
#include <type_traits>


namespace HAL
{


/********************************** Templates *******************************************/
/**
 * \brief template to enable/disable bitwise operator overloading for a scoped enum. Default is Disabled
 * 
 * \tparam Enum the enumeration to enable/disable
 */
template <typename Enum>
struct EnableBitwiseOperators
{
   static const bool enable = false;
};

/**
 * \brief template operator overload of bitwise OR
 * 
 * \tparam Enum 
 * \param first argument to OR
 * \param second argument to OR
 * \retval std::enable_if_t<EnableBitwiseOperators<Enum>::enable, Enum> 
 */
template <typename Enum>
typename std::enable_if_t<EnableBitwiseOperators<Enum>::enable, Enum> operator |(Enum first, Enum second)
{
   using underlying_type = std::underlying_type_t<Enum>;
   return static_cast<underlying_type>(first) | static_cast<underlying_type>(second);
}

/**
 * \brief template operator overload of bitwise OR
 * 
 * \tparam Enum 
 * \param first argument to AND
 * \param second argument to AND
 * \retval std::enable_if_t<EnableBitwiseOperators<Enum>::enable, Enum> 
 */
template <typename Enum>
typename std::enable_if_t<EnableBitwiseOperators<Enum>::enable, Enum> operator &(Enum first, Enum second)
{
   using underlying_type = std::underlying_type_t<Enum>;
   return static_cast<underlying_type>(first) & static_cast<underlying_type>(second);
}

}; // namespace HAL

#endif /* __HAL_BITWISE_OPS_H__ */
