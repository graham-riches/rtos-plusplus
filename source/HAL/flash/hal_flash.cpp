// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Graham Riches

/********************************** Includes *******************************************/
#include "hal_flash.hpp"
#include "hal_utilities.hpp"

namespace HAL::flash
{
/****************************** Function Definitions ***********************************/

void set_access_control_register_bit(access_control_register bit, uint8_t value) {
    switch ( bit ) {
        // Handle the single bit width registers in one case
        case access_control_register::data_cache_enable:
        case access_control_register::prefetch_enable:
        case access_control_register::instruction_cache_enable:
        case access_control_register::data_cache_reset:
        case access_control_register::instruction_cache_reset:
            clear_bits(FLASH->ACR, 0x01u << static_cast<uint32_t>(bit));
            if ( value > 0 ) {
                set_bits(FLASH->ACR, 0x01u << static_cast<uint32_t>(bit));
            }
            break;

        // Latency is a special case of 4-bits wide
        case access_control_register::latency:
            clear_bits(FLASH->ACR, 0x0Fu << static_cast<uint32_t>(bit));
            set_bits(FLASH->ACR, (value & 0x0Fu) << static_cast<uint32_t>(bit));
            break;

        default:
            break;
    }
}
};  // namespace HAL::flash
