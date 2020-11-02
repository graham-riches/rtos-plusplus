/*! \file hal_interrupt.h
*
*  \brief hal_interrupt C++ API to interface with all system interrupts.
*  \details registers a specific function callback with context (aimed for C++)
*           in a interrupts vector stored in SRAM. Each low-level interrupt function is
*           manually defined, which maintains the ability of the NVIC to handle priority
*           but each function just calls a registered ISR out of the same array.
*
*
*  \author Graham Riches
*/

#ifndef __HAL_INTERRUPT_H__
#define __HAL_INTERRUPT_H__

/********************************** Includes *******************************************/
#include "common.h"

namespace HAL
{
namespace Interrupt
{
/*********************************** Consts ********************************************/

/************************************ Types ********************************************/
/**
 * \brief enumeration of all interrupts
 * 
 */
enum class InterruptName : unsigned
{
   /* Cortex M- internal interrupts */
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

   /* external interrupts - hardware specific */
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
   reserved_5,
   hash_random_number,
   floating_point_unit,
};

/**
 * \brief base class for peripherals with interrupts
 * \details this should be inherited to create specific peripheral
 *          classes that have interrupts
 */
class InterruptPeripheral
{
   public:
   virtual void irq_handler( uint8_t irq_type ) { PARAMETER_NOT_USED( irq_type ); }
};

/**
 * \brief structure to register a peripherals interrupt handler in the interrupt table
 * 
 */
typedef struct
{
   InterruptPeripheral *peripheral;  //!< pointer to the peripheral
   uint8_t irq_type;                 //!< interrupt type (for peripherals with multiple interrupts)
} InterruptHandler;

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/

/****************************** Functions Prototype ************************************/
void register_callback( InterruptName interrupt, InterruptPeripheral *peripheral, uint8_t type, uint32_t priority );
void set_systick_frequency( uint32_t ticks );

};  // namespace Interrupt
};  // namespace HAL

#endif /* __HAL_INTERRUPT_H__ */
