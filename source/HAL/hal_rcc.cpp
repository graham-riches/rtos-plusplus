/*! \file hal_rcc.cpp
*
*  \brief HAL++ reset and control clock abstration layer
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "hal_rcc.h"
#include "hal_utilities.h"

namespace HAL::rcc
{
/************************************ Types ********************************************/
/**
 * \brief Structure to store clock speeds
 */
struct ClockSpeed {
    uint32_t system_clock;
    uint32_t ahb;
    uint32_t apb1;
    uint32_t apb2;
    uint8_t ahb_scaler;
    uint8_t apb1_scaler;
    uint8_t apb2_scaler;
};

/************************************ Function Declarations ********************************************/
static void save_clock_configuration();

/************************************ Local Variables ********************************************/
// Stores the clock configuration of the system
static ClockSpeed g_clock_configuration = {
    .system_clock = 1,
    .ahb = 1,
    .apb1 = 1,
    .apb2 = 1,
    .ahb_scaler = 1,
    .apb1_scaler = 1,
    .apb2_scaler = 1,
};

/************************************ Function Definitions ********************************************/
/**
 * \brief Save the current clock speed
 */
static void save_clock_configuration() {
    g_clock_configuration.ahb = g_clock_configuration.system_clock / g_clock_configuration.ahb_scaler;
    g_clock_configuration.apb1 = g_clock_configuration.system_clock / g_clock_configuration.apb1_scaler;
    g_clock_configuration.apb2 = g_clock_configuration.system_clock / g_clock_configuration.apb2_scaler;
}

//!< Get a control register value
uint8_t get_control_register(control_register reg) {
    uint32_t mask = 0;

    switch ( reg ) {
        // Intentional fallthrough
        case control_register::hsi_on:
        case control_register::hsi_ready:
        case control_register::hse_on:
        case control_register::hse_ready:
        case control_register::hse_bypass:
        case control_register::clock_security:
        case control_register::main_pll_on:
        case control_register::main_pll_ready:
        case control_register::i2s_pll_on:
        case control_register::i2s_pll_ready:
            mask = static_cast<uint32_t>((0x01u << static_cast<uint32_t>(reg)));
            break;

        case control_register::hsi_trim:
            mask = static_cast<uint32_t>((0xFFu << static_cast<uint32_t>(reg)));
            break;

        case control_register::hsi_cal:
            mask = static_cast<uint32_t>((0x1Fu << static_cast<uint32_t>(reg)));
            break;

        default:
            break;
    }

    uint8_t value = static_cast<uint8_t>((RCC->CR & mask) >> static_cast<uint8_t>(reg));
    return value;
}

//!< Set a control register value
void set_control_register(control_register reg, uint8_t value) {
    set_bits(RCC->CR, value << static_cast<uint8_t>(reg));
}

//!< Configure the main phase-locked loop
void configure_main_pll(pll_source clock_source,
                        uint32_t oscillator_speed,
                        uint8_t pll_m,
                        uint16_t pll_n,
                        pll_output_prescaler pll_p,
                        uint8_t pll_q) {
    // Clear the register
    RCC->PLLCFGR = 0;

    // Set the phase locked loop clock source
    set_bits(RCC->PLLCFGR, static_cast<uint8_t>(clock_source) << static_cast<uint8_t>(pll_register::pll_source));

    // Set the main division factor for the system clock: PLL_P
    set_bits(RCC->PLLCFGR, static_cast<uint8_t>(pll_p) << static_cast<uint8_t>(pll_register::pll_p));

    // Set the main multiplication factor for the system clock: PLL_N
    set_bits(RCC->PLLCFGR, pll_n << static_cast<uint8_t>(pll_register::pll_n));

    // Set the division factor for the entry to the VCO: PLL_M
    set_bits(RCC->PLLCFGR, pll_m << static_cast<uint8_t>(pll_register::pll_m));

    // Set the output for the 48 MHz clock source: PLL_Q
    set_bits(RCC->PLLCFGR, pll_q << static_cast<uint8_t>(pll_register::pll_q));

    // Set the system clock speed
    uint8_t pll_p_scaler = (static_cast<uint8_t>(pll_p) + 1) * 2;
    g_clock_configuration.system_clock = (oscillator_speed * pll_n) / (pll_m * pll_p_scaler);
    save_clock_configuration();
}

//!< Set the main system clock to source
void set_system_clock_source(system_clock_source source) {
    uint32_t clock_source_mask = 0b11;
    clear_bits(RCC->CFGR, clock_source_mask);
    set_bits(RCC->CFGR, static_cast<uint8_t>(source) << static_cast<uint8_t>(configuration_register::system_clock_source));

    // Wait until the clock source status register matches the selected clock
    while ( (RCC->CFGR & clock_source_mask) != static_cast<uint32_t>(source) ) {
    }
}

//!< Configure ahb clock bus
void configure_ahb_clock(ahb_prescaler prescaler) {
    set_bits(RCC->CFGR, static_cast<uint8_t>(prescaler) << static_cast<uint8_t>(configuration_register::ahb_prescaler));
    // clang-format off
    g_clock_configuration.ahb_scaler = (prescaler == ahb_prescaler::prescaler_none) ? 1
                                     : (prescaler == ahb_prescaler::prescaler_2)    ? 2
                                     : (prescaler == ahb_prescaler::prescaler_4)    ? 4
                                     : (prescaler == ahb_prescaler::prescaler_8)    ? 8
                                     : (prescaler == ahb_prescaler::prescaler_16)   ? 16
                                     : (prescaler == ahb_prescaler::prescaler_64)   ? 64
                                     : (prescaler == ahb_prescaler::prescaler_128)  ? 128
                                     : (prescaler == ahb_prescaler::prescaler_256)  ? 256
                                     : (prescaler == ahb_prescaler::prescaler_512)  ? 512
                                     : 0;
    // clang-format on            
    save_clock_configuration();
}

//!< Configure APB2 clock bus
void configure_apb2_clock(apb_prescaler prescaler) {
    set_bits(RCC->CFGR, static_cast<uint8_t>(prescaler) << static_cast<uint8_t>(configuration_register::apb2_prescaler));        
    // clang-format off
    g_clock_configuration.apb2_scaler = (prescaler == apb_prescaler::prescaler_none) ? 1
                                      : (prescaler == apb_prescaler::prescaler_2)    ? 2
                                      : (prescaler == apb_prescaler::prescaler_4)    ? 4
                                      : (prescaler == apb_prescaler::prescaler_8)    ? 8
                                      : (prescaler == apb_prescaler::prescaler_16)   ? 16
                                      : 0;
    // clang-format on    
    save_clock_configuration();
}

//!< Configure APB1 clock bus
void configure_apb1_clock(apb_prescaler prescaler) {
    set_bits(RCC->CFGR, static_cast<uint8_t>(prescaler) << static_cast<uint8_t>(configuration_register::apb1_prescaler));    
    // clang-format off
    g_clock_configuration.apb1_scaler = (prescaler == apb_prescaler::prescaler_none) ? 1
                                      : (prescaler == apb_prescaler::prescaler_2)    ? 2
                                      : (prescaler == apb_prescaler::prescaler_4)    ? 4
                                      : (prescaler == apb_prescaler::prescaler_8)    ? 8
                                      : (prescaler == apb_prescaler::prescaler_16)   ? 16
                                      : 0;
    // clang-format on 
    save_clock_configuration();
}

//!< Enable an AHB clock
void set_ahb_clock(ahb1_clocks clock, bool enable) {
    if ( enable ) {
        set_bits(RCC->AHB1ENR, static_cast<uint8_t>(enable) << static_cast<uint8_t>(clock));        
    } else {
        clear_bits(RCC->AHB1ENR, static_cast<uint8_t>(enable) << static_cast<uint8_t>(clock));        
    }
}

//!< Enable an AHB clock
void set_ahb_clock(ahb2_clocks clock, bool enable) {
    if ( enable ) {
        set_bits(RCC->AHB2ENR, static_cast<uint8_t>(enable) << static_cast<uint8_t>(clock));        
    } else {
        clear_bits(RCC->AHB2ENR, static_cast<uint8_t>(enable) << static_cast<uint8_t>(clock));        
    }
}

//!< Enable an AHB clock
void set_ahb_clock(ahb3_clocks clock, bool enable) {
    if ( enable ) {
        set_bits(RCC->AHB3ENR, static_cast<uint8_t>(enable) << static_cast<uint8_t>(clock));    
    } else {
        clear_bits(RCC->AHB3ENR, static_cast<uint8_t>(enable) << static_cast<uint8_t>(clock));        
    }
}

//!< Enable an APB clock
void set_apb_clock(apb1_clocks clock, bool enable) {
    if ( enable ) {
        set_bits(RCC->APB1ENR, static_cast<uint8_t>(enable) << static_cast<uint8_t>(clock));
    } else {
        clear_bits(RCC->APB1ENR, static_cast<uint8_t>(enable) << static_cast<uint8_t>(clock));        
    }
}

//!< Enable an APB clock
void set_apb_clock(apb2_clocks clock, bool enable) {
    if ( enable ) {
        set_bits(RCC->APB2ENR, static_cast<uint8_t>(enable) << static_cast<uint8_t>(clock));        
    } else {
        clear_bits(RCC->APB2ENR, static_cast<uint8_t>(enable) << static_cast<uint8_t>(clock));        
    }
}

//!< Get a clock speed
uint32_t get_clock_speed(clocks clock) {
    uint32_t clock_speed = g_clock_configuration.system_clock;
    switch ( clock ) {
        // intentional fallthrough
        case clocks::AHB1:
        case clocks::AHB2:
        case clocks::AHB3:
            clock_speed = g_clock_configuration.ahb;
            break;

        case clocks::APB1:
            clock_speed = g_clock_configuration.apb1;
            break;

        case clocks::APB2:
            clock_speed = g_clock_configuration.apb2;
            break;

        default:
            break;
    }
    return clock_speed;
}

};  // namespace HAL::rcc