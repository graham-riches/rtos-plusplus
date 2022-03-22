/*! \file peripherals.cpp
*
*  \brief HAL setup and abstraction for all peripherals
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "peripherals.h"
#include "hal_flash.h"
#include "hal_interrupt.h"
#include "hal_power.h"
#include "hal_rcc.h"
#include "os.h"

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
/**
* \brief    setup all of the peripherals
*
* \param    None
* \retval   None
*/
void initialize_peripherals(void) {
    /* initialize the debug UART and print a welcome banner */
    //debug_port.initialize();

    /* print out a splash screen */
    //debug_port.send("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    //debug_port.send("STM32F407 Debug Terminal \n");
    //debug_port.send("   Author: Graham Riches \n");
    //debug_port.send("   Date: November 1, 2020 \n");
    //debug_port.send("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

    /* turn on the LEDs for status indication */
    green_led.set(true);
    orange_led.set(false);
    red_led.set(true);
    blue_led.set(false);

    /* initialize the accelerometer */
    //accelerometer.initialize();
}
