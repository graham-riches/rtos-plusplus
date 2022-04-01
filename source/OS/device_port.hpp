/**
 * \file device_port.hpp
 * \author Graham Riches
 * \brief Manages the hardware porting to a specific device
 * \version 0.1
 * \date 2022-03-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

/********************************** Includes *******************************************/
#include "hal_nvic.hpp"
#include <cstdint>

/********************************** Macros *******************************************/
#define DISABLE_INTERRUPTS() __asm("CPSID I\n");
#define ENABLE_INTERRUPTS()  __asm("CPSIE I\n");

/********************************** Types *******************************************/
/**
 * \brief enumeration of all interrupts for STM32F4xx
 */
enum class stm32f4_irq : unsigned {
    // Internal Cortex Interrupts
    reset_handler = 1,
    nmi_handler,
    hardfault_handler,
    memmanage_handler,
    busfault_handler,
    usagefault_handler,
    reserved_0,
    reserved_1,
    reserved_2,
    reserved_3,
    svc_handler,
    debugmon_handler,
    reserved_4,
    pendsv_handler,
    systick_handler,

    // Peripheral interrupts
    window_watchdog,
    power_voltage_detection,
    time_stamp,
    rtc_wakeup,
    flash,
    rcc,
    exti_0,
    exti_1,
    exti_2,
    exti_3,
    exti_4,
    dma_1_stream_0,
    dma_1_stream_1,
    dma_1_stream_2,
    dma_1_stream_3,
    dma_1_stream_4,
    dma_1_stream_5,
    dma_1_stream_6,
    adc,
    can_1_tx,
    can_1_rx_0,
    can_1_rx_1,
    can_1_sce,
    exti_9_5,
    timer_1_break,
    timer_1_update,
    timer_1_trigger_commutation,
    timer_1_capture_compare,
    timer_2,
    timer_3,
    timer_4,
    i2c_1_event,
    i2c_1_error,
    i2c_2_event,
    i2c_2_error,
    spi_1,
    spi_2,
    usart_1,
    usart_2,
    usart_3,
    exti_15_10,
    rtc_alarm,
    otg_fs_wakeup,
    timer_8_break,
    timer_8_update,
    timer_8_trigger_commutation,
    timer_8_capture_compare,
    dma_1_stream_7,
    fsmc,
    sdio,
    timer_5,
    spi_3,
    uart_4,
    uart_5,
    timer_6_dac,
    timer_7,
    dma_2_stream_0,
    dma_2_stream_1,
    dma_2_stream_2,
    dma_2_stream_3,
    dma_2_stream_4,
    ethernet,
    ethernet_wakeup,
    can_2_tx,
    can_2_rx_0,
    can_2_rx_1,
    can_2_sce,
    otg_fs,
    dma_2_stream_5,
    dma_2_stream_6,
    dma_2_stream_7,
    usart_6,
    i2c_3_event,
    i2c_3_error,
    otg_hs_endpoint_1_out,
    otg_hs_endpoint_1_in,
    otg_hs_wakeup,
    otg_hs,
    dcmi,
    cryptography_unit,
    hash_random_number,
    floating_point_unit,
};

//!< Helper alias for maximum interrupt number
constexpr std::size_t STM32F4_TOTAL_ISR = static_cast<std::size_t>(stm32f4_irq::floating_point_unit);

/********************************** Function Declarations *******************************************/
/**
 * \brief Set a pending context switch interrupt (platform dependent)
 * 
 */
void set_pending_context_switch();

/**
 * \brief check if a context switch is pending (platform dependent)
 * 
 * \retval true/false
 */
bool is_context_switch_pending();

/********************************** Used Interrupts *******************************************/
void usart3_irqn();