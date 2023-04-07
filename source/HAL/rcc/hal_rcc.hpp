// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Graham Riches

#pragma once

/********************************** Includes *******************************************/
#include "stm32f4xx.h"

namespace HAL::rcc
{

/************************************ Types ********************************************/
/**
 * \brief Enumeration of RCC clocks
 */
enum class clocks : unsigned {
    AHB1 = 0,  //!< clock speed for AHB1 bus
    AHB2,      //!< clock speed for AHB2 bus
    AHB3,      //!< clock speed for AHB3 bus
    APB1,      //!< clock speed for APB1 bus
    APB2,      //!< clock speed for APB2 bus
};

/**
 * \brief main control registers bit offsets for the reset and control clock
 */
enum class control_register : unsigned {
    hsi_on = 0,           //!< internal high speed oscillator enable
    hsi_ready = 1,        //!< internal high speed oscillator is ready
    hsi_trim = 3,         //!< start address for the internal high speed oscillator trim adjustment
    hsi_cal = 8,          //!< start address for the internal high speed oscillator calibration
    hse_on = 16,          //!< external high speed oscillator enable
    hse_ready = 17,       //!< external high speed oscillator is ready
    hse_bypass = 18,      //!< bypass the external oscillator
    clock_security = 19,  //!< enable the clock security system
    main_pll_on = 24,     //!< enable the main phase locked loop
    main_pll_ready = 25,  //!< main phase locked loop is ready
    i2s_pll_on = 26,      //!< enable the i2s phase locked loop
    i2s_pll_ready = 27,   //!< i2s phase locked loop is available
};

/**
 * \brief enumeration of register bit offsets for the PLLCFGR register
 */
enum class pll_register : unsigned {
    pll_m = 0,        //!< bit locations of the PLL_M factor register
    pll_n = 6,        //!< bit locations of the PLL_N factor register
    pll_p = 16,       //!< bit locations of the PLL_P factor register
    pll_source = 22,  //!< bit locations of the PLL input source selector register
    pll_q = 24        //!< bit locations of the PLL_Q factor register
};

/**
 * \brief clock source selector for the main PLL
 */
enum class pll_source : unsigned {
    high_speed_internal = 0x00,
    high_speed_external = 0x01,
};

/**
 * \brief prescaler modes for the main phase locked loop output divisor (PLL_P)
 */
enum class pll_output_prescaler : unsigned { prescaler_2 = 0x00, prescaler_4 = 0x01, prescaler_6 = 0x02, prescaler_8 = 0x03 };

/**
 * \brief enumeration of register bit offsets for the RCC configuration register
 * \todo this may require a renaming depending on what other peripherals get added to the HAL
 */
enum class configuration_register : unsigned {
    system_clock_source = 0,          //!< select the main system clock source
    sysem_clock_status = 2,           //!< status of the main system clock (which is configured)
    ahb_prescaler = 4,                //!< prescaler for the high performance bus
    apb1_prescaler = 10,              //!< prescaler for the lower speed peripheral bus clock
    apb2_prescaler = 13,              //!< prescaler for the higher speed peripheral bus clock
    rtc_prescaler = 16,               //!< prescaler for the real-time clock
    output_one_clock_source = 21,     //!< output clock signal source select
    i2s_clock_source = 23,            //!< i2s clock source select
    output_one_clock_prescaler = 24,  //!< output clock one signal prescaler
    output_two_clock_prescaler = 27,  //!< output clock two signal prescaler
    output_two_clock_source = 30      //!< output clock two source select
};

/**
 * \brief enumeration of system clock sources
 */
enum class system_clock_source : unsigned { high_speed_internal = 0, high_speed_external = 1, phase_locked_loop = 2, none = 3 };

/**
 * \brief prescalers for the high speed bus/system clock
 */
enum class ahb_prescaler : unsigned {
    prescaler_none = 0b0000,
    prescaler_2 = 0b1000,
    prescaler_4 = 0b1001,
    prescaler_8 = 0b1010,
    prescaler_16 = 0b1011,
    prescaler_64 = 0b1100,
    prescaler_128 = 0b1101,
    prescaler_256 = 0b1110,
    prescaler_512 = 0b1111
};

/**
 * \brief prescalers for the peripheral busses
 */
enum class apb_prescaler : unsigned {
    prescaler_none = 0b000,
    prescaler_2 = 0b100,
    prescaler_4 = 0b101,
    prescaler_8 = 0b110,
    prescaler_16 = 0b111,
};

/**
 * \brief enumeration of bit offsets for ahb1 clocks
 */
enum class ahb1_clocks : unsigned {
    gpio_a = 0,
    gpio_b = 1,
    gpio_c = 2,
    gpio_d = 3,
    gpio_e = 4,
    gpio_f = 5,
    gpio_g = 6,
    gpio_h = 7,
    gpio_i = 8,
    crc = 12,
    backup_sram = 18,
    ccm_data_ram = 20,
    dma_1 = 21,
    dma_2 = 22,
    ethernet_mac = 25,
    ethernet_mac_tx = 26,
    ethernet_mac_rx = 27,
    ethernet_ptp = 28,
    usb_otg = 29,
    sb_otg_hsulpi = 30
};

/**
 * \brief enumeration of bit offsets for ahb2 clocks
 */
enum class ahb2_clocks : unsigned {
    digital_camera_interface = 0,
    cryptography = 4,
    hash = 5,
    random_number_generator = 6,
    usb_otg_fs = 7,
};

/**
 * \brief enumeration of bit offsets for ahb2 clocks
 */
enum class ahb3_clocks : unsigned {
    static_memory_controller = 0,
};

/**
 * \brief enumeration of bit offsets for apb1 clocks
 */
enum class apb1_clocks : unsigned {
    timer_2 = 0,
    timer_3 = 1,
    timer_4 = 2,
    timer_5 = 3,
    timer_6 = 4,
    timer_7 = 5,
    timer_12 = 6,
    timer_13 = 7,
    timer_14 = 8,
    window_watchdog = 11,
    spi_2 = 14,
    spi_3 = 15,
    usart_2 = 17,
    usart_3 = 18,
    uart_4 = 19,
    uart_5 = 20,
    i2c_1 = 21,
    i2c_2 = 22,
    i2c_3 = 23,
    can_1 = 25,
    can_2 = 26,
    power_management = 28,
    dac = 29,
};

/**
 * \brief enumeration of bit offsets for apb2 clocks
 */
enum class apb2_clocks : unsigned {
    timer_1 = 0,
    timer_8 = 1,
    usart_1 = 4,
    usart_6 = 5,
    adc_1 = 8,
    adc_2 = 9,
    adc_3 = 10,
    sdio = 11,
    spi_1 = 12,
    sys_config = 14,
    timer_9 = 16,
    timer_10 = 17,
    timer_11 = 18
};

/****************************** Function Declarations ************************************/
/**
 * \brief Get a control register value
 * 
 * \param register the register to get
 * \retval register value     
 */
uint8_t get_control_register(control_register reg);

/**
 * \brief Set a control register value
 * 
 * \param register the register to set
 * \param value to set
 */
void set_control_register(control_register reg, uint8_t value);

/**
 * \brief setup the main PLL
 * 
 * \param clock_source clock source (HSI, HSE)
 * \param oscillator_speed how fast the oscillator is running
 * \param pll_m m prescaler
 * \param pll_n n prescaler
 * \param pll_p p prescaler
 * \param pll_q q prescaler
 */
void configure_main_pll(pll_source clock_source,
                        uint32_t oscillator_speed,
                        uint8_t pll_m,
                        uint16_t pll_n,
                        pll_output_prescaler pll_p,
                        uint8_t pll_q);

/**
 * \brief Set the system clock
 * 
 * \param source the clock source for the system
 */
void set_system_clock_source(system_clock_source source);

/**
 * \brief configure the ahb clock prescaler
 * 
 * \param prescaler Clock multiplier
 */
void configure_ahb_clock(ahb_prescaler prescaler);

/**
 * \brief configure the apb2 bus prescaler
 * 
 * \param prescaler Clock multiplier
 */
void configure_apb2_clock(apb_prescaler prescaler);

/**
 * \brief configure the apb1 clock prescaler
 * 
 * \param prescaler Clock multiplier
 */
void configure_apb1_clock(apb_prescaler prescaler);

/**
 * \brief enable or disable a clock source on the ahb1 bus
 * 
 * \param clock the clock to enable/disable
 * \param enable state
 */
void set_ahb_clock(ahb1_clocks clock, bool enable);

/**
 * \brief enable or disable a clock source on the ahb2 bus
 * 
 * \param clock the clock to enable/disable
 * \param enable state
 */
void set_ahb_clock(ahb2_clocks clock, bool enable);

/**
 * \brief enable or disable a clock source on the ahb3 bus
 * 
 * \param clock the clock to enable/disable
 * \param enable state
 */
void set_ahb_clock(ahb3_clocks clock, bool enable);

/**
 * \brief enable or disable a clock source on the apb1 bus
 * 
 * \param clock the clock to enable/disable
 * \param enable state
 */
void set_apb_clock(apb1_clocks clock, bool enable);

/**
 * \brief enable or disable a clock source on the apb2 bus
 * 
 * \param clock the clock to enable/disable
 * \param enable state
 */
void set_apb_clock(apb2_clocks clock, bool enable);

/**
 * \brief Get the clock speed for a specific bus
 * 
 * \param clock the clock bus
 * \retval uint32_t speed in Hz
 */
uint32_t get_clock_speed(clocks clock);
};  // namespace HAL::rcc
