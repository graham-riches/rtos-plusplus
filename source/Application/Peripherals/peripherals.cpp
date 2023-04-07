// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Graham Riches

/********************************** Includes *******************************************/
#include "peripherals.hpp"
#include "hal_flash.hpp"
#include "hal_nvic.hpp"
#include "hal_power.hpp"
#include "hal_rcc.hpp"
#include "os.hpp"
#include "debug_port.hpp"

/******************************* Global Variables **************************************/
using namespace HAL;
gpio::output_pin green_led(GPIOD,
                           rcc::ahb1_clocks::gpio_d,
                           gpio::pin_id::pin_12,
                           gpio::pin_mode::output,
                           gpio::pin_speed::low,
                           gpio::pull_mode::pull_down,
                           gpio::output_mode::push_pull);

gpio::output_pin orange_led(GPIOD,
                            rcc::ahb1_clocks::gpio_d,
                            gpio::pin_id::pin_13,
                            gpio::pin_mode::output,
                            gpio::pin_speed::low,
                            gpio::pull_mode::pull_down,
                            gpio::output_mode::push_pull);

gpio::output_pin red_led(GPIOD,
                         rcc::ahb1_clocks::gpio_d,
                         gpio::pin_id::pin_14,
                         gpio::pin_mode::output,
                         gpio::pin_speed::low,
                         gpio::pull_mode::pull_down,
                         gpio::output_mode::push_pull);

gpio::output_pin blue_led(GPIOD,
                          rcc::ahb1_clocks::gpio_d,
                          gpio::pin_id::pin_15,
                          gpio::pin_mode::output,
                          gpio::pin_speed::low,
                          gpio::pull_mode::pull_down,
                          gpio::output_mode::push_pull);

/****************************** Function Definitions ***********************************/
void initialize_peripherals() {
    debug_port.initialize();        

    // Turn on LEDs
    green_led.set(true);
    orange_led.set(false);
    red_led.set(true);
    blue_led.set(false);
}
