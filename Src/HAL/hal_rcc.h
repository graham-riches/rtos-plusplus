/*! \file hal_rcc.h
*
*  \brief HAL++ library implementation of STM32F4 cortex M4 reset and control clock
*
*
*  \author Graham Riches
*/

#ifndef __HAL_RCC_H__
#define __HAL_RCC_H__

/********************************** Includes *******************************************/
#include "stm32f4xx.h"
#include "hal_bitwise_operators.h"
#include <stdint.h>


namespace HAL
{
namespace ResetControlClock
{
/*********************************** Consts ********************************************/

/************************************ Types ********************************************/
/**
 * \brief main control registers bit offsets for the reset and control clock
 * 
 */
enum class RCCRegister : unsigned
{
   hsi_on =  0,         //!< internal high speed oscillator enable
   hsi_ready = 1,       //!< internal high speed oscillator is ready
   hsi_trim = 3,        //!< start address for the internal high speed oscillator trim adjustment
   hsi_cal = 8,         //!< start address for the internal high speed oscillator calibration
   hse_on = 16,         //!< external high speed oscillator enable
   hse_ready = 17,      //!< external high speed oscillator is ready
   hse_bypass = 18,     //!< bypass the external oscillator
   clock_security = 19, //!< enable the clock security system 
   main_pll_on = 24,    //!< enable the main phase locked loop
   main_pll_ready = 25, //!< main phase locked loop is ready
   i2s_pll_on = 26,     //!< enable the i2s phase locked loop
   i2s_pll_ready = 27,  //!< i2s phase locked loop is available
};

/**
 * \brief enumeration of register bit offsets for the PLLCFGR register
 * 
 */
enum class PLLRegister : unsigned
{
   pll_m = 0,       //!< bit locations of the PLL_M factor register
   pll_n = 6,       //!< bit locations of the PLL_N factor register
   pll_p = 16,      //!< bit locations of the PLL_P factor register
   pll_source = 22, //!< bit locations of the PLL input source selector register
   pll_q = 24       //!< bit locations of the PLL_Q factor register
};

/**
 * \brief clock source selector for the main PLL
 * 
 */
enum class PLLSource : unsigned
{
   high_speed_internal = 0x00,
   high_speed_external = 0x01,
};

/**
 * \brief prescaler modes for the main phase locked loop output divisor (PLL_P)
 * 
 */
enum class PLLOutputPrescaler : unsigned
{
   prescaler_2 = 0x00,
   prescaler_4 = 0x01,
   prescaler_6 = 0x02,
   prescaler_8 = 0x03
};

/**
 * \brief enumeration of register bit offsets for the RCC configuration register
 * 
 */
enum class ConfigurationRegister : unsigned
{
   system_clock_source = 0,         //!< select the main system clock source
   sysem_clock_status = 2,          //!< status of the main system clock (which is configured)
   ahb_prescaler = 4,               //!< prescaler for the high performance bus
   apb1_prescaler = 10,             //!< prescaler for the lower speed peripheral bus clock
   apb2_prescaler = 13,             //!< prescaler for the higher speed peripheral bus clock
   rtc_prescaler = 16,              //!< prescaler for the real-time clock
   output_one_clock_source = 21,    //!< output clock signal source select
   i2s_clock_source = 23,           //!< i2s clock source select
   output_one_clock_prescaler = 24, //!< output clock one signal prescaler
   output_two_clock_prescaler = 27, //!< output clock two signal prescaler
   output_two_clock_source = 30     //!< output clock two source select
};

/**
 * \brief enumeration of system clock sources
 * 
 */
enum class SystemClockSource : unsigned
{
   high_speed_internal = 0,
   high_speed_external = 1,
   phase_locked_loop = 2,
   none = 3
};

/**
 * \brief prescalers for the high speed bus/system clock
 * 
 */
enum class AHBPrescaler : unsigned
{
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
 * 
 */
enum class APBPrescaler : unsigned
{
   prescaler_none = 0b000,
   prescaler_2 = 0b100,
   prescaler_4 = 0b101,
   prescaler_8 = 0b110,
   prescaler_16 = 0b111,
};

/*********************************** Macros ********************************************/

/****************************** Function Declarations ************************************/
uint8_t get_control_register( RCCRegister reg );
void    set_control_register( RCCRegister reg, uint8_t value );
void    configure_main_pll( PLLSource clock_source, uint8_t pll_m, uint16_t pll_n, PLLOutputPrescaler pll_p, uint8_t pll_q );
void    set_system_clock_source( SystemClockSource source );
void    configure_ahb_clock( AHBPrescaler prescaler );
void    configure_apb2_clock( APBPrescaler prescaler );
void    configure_apb1_clock( APBPrescaler prescaler );


};  // namespace ResetControlClock
};  // namespace HAL

#endif /* __HAL_RCC_H__ */
