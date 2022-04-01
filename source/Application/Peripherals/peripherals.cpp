/*! \file peripherals.cpp
*
*  \brief HAL setup and abstraction for all peripherals
*
*
*  \author Graham Riches
*/

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
    debug_port.log_message("test\n");
    //debug_port.log_message("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    //debug_port.log_message("STM32F407 Debug Terminal \n");
    //debug_port.log_message("   Author: Graham Riches \n");
    //debug_port.log_message("   Date: November 1, 2020 \n");
    //debug_port.log_message("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

    // Turn on LEDs
    green_led.set(true);
    orange_led.set(false);
    red_led.set(true);
    blue_led.set(false);

    /* initialize the accelerometer */
    //accelerometer.initialize();
}
