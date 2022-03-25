/*! \file debug_port.c
*
*  \brief class for managing the debug port.
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "debug_port.hpp"
#include "hal_gpio.hpp"
#include "hal_nvic.hpp"
#include "hal_rcc.hpp"

/******************************* Global Variables **************************************/
using namespace HAL::gpio;

DebugPort<128> debug_port(USART3,
                          HAL::gpio::alternate_mode_pin{GPIOB,
                                                        HAL::rcc::ahb1_clocks::gpio_b,
                                                        pin_id::pin_10,
                                                        pin_mode::alternate,
                                                        pin_speed::very_high,
                                                        pull_mode::pull_up,
                                                        output_mode::push_pull,
                                                        alternate_mode::af7},
                          HAL::gpio::alternate_mode_pin{GPIOB,
                                                        HAL::rcc::ahb1_clocks::gpio_b,
                                                        pin_id::pin_11,
                                                        pin_mode::alternate,
                                                        pin_speed::very_high,
                                                        pull_mode::pull_up,
                                                        output_mode::push_pull,
                                                        alternate_mode::af7});
