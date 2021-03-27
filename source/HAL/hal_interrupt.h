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
#include "stm32f4xx.h"

namespace HAL
{

/*********************************** Consts ********************************************/
constexpr uint32_t nvic_priority_group_level = 0; //!< enable 16 configurable priority levels and no subpriorities
constexpr uint32_t active_vector_mask = 0xFF;     //!< mask for currently active interrupt vector
constexpr uint8_t external_interrupt_offset = 16; //!< offset for mappping interrupt numbers to enums defined below

/************************************ Types ********************************************/
/**
 * \brief enumeration of all interrupts
 * 
 */
enum class InterruptName : unsigned {
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
 * \brief enumeration of preemption priority groupings.
 * \note an interrupt in the highest priority grouping (level_1) cannot be interrupt by a lower priory grouping interrpt
 */
enum class PreemptionPriority : unsigned {
    level_1 = 0,
    level_2,
    level_3,
    level_4,
    level_5,
    level_6,
    level_7,
    level_8,
    level_9,
    level_10,
    level_11,
    level_12,
    level_13,
    level_14,
    level_15,
    level_16,
};

/**
 * \brief pure virtual base class for peripherals with interrupts
 * \details this should be inherited to create specific peripheral
 *          classes that have interrupts
 */
class InterruptPeripheral {
  public:
    virtual void irq_handler(uint8_t irq_type) = 0;
};

/**
 * \brief structure to register a peripherals interrupt handler in the interrupt table
 * 
 */
typedef struct {
    InterruptPeripheral* peripheral;  //!< pointer to the peripheral
    uint8_t irq_type;                 //!< interrupt type (for peripherals with multiple interrupts)
} InterruptHandler;

/**
 * \brief class to manage all peripheral interrupts
 * 
 */
class InterruptManager {
  public:
    /**
     * \brief Construct a new Interrupt Manager:: Interrupt Manager object
     * \note this will configure the microcontroller NVIC priority grouping
     */
    InterruptManager() {
        NVIC_SetPriorityGrouping(nvic_priority_group_level);
    }

    /**
     * \brief set the priority grouping and subpriority for an interrupt
     * 
     * \param interrupt the interrupt to set the priority for
     * \param preemption_priority premption priority level
     */
    inline void set_priority(InterruptName interrupt, PreemptionPriority preemption_priority) {
        uint32_t priority = NVIC_EncodePriority(nvic_priority_group_level, static_cast<uint32_t>(preemption_priority), 1UL);
        IRQn external_interrupt_number = static_cast<IRQn>(static_cast<uint8_t>(interrupt) - external_interrupt_offset);
        NVIC_SetPriority(external_interrupt_number, priority);
        NVIC_EnableIRQ(external_interrupt_number);
    }

    /**
     * \brief register an interrupt service routing from a peripheral to an instance
     *        in the ISR handler table
     * 
     * \param interrupt the interrupt to register
     * \param peripheral pointer to the derived peripheral with interrupt capabilities
     * \param type type of the interrupt (for peripherals with multiple interrupts)
     * \param premption_priority The NVIC preemption priority
     * \param subpriority the interrupt sub-priority
     */
    inline void register_callback(InterruptName interrupt,
                           InterruptPeripheral* peripheral,
                           uint8_t type,
                           PreemptionPriority preemption_priority) {
        /* fill the handler structure */
        InterruptHandler handler;
        handler.peripheral = peripheral;
        handler.irq_type = type;

        /* place it in the handler array */
        this->isr_table[static_cast<uint8_t>(interrupt)] = handler;

        /* set the priority level */
        set_priority(interrupt, preemption_priority);
    }
    
    /**
     * \brief default ISR handler to map specific function pointers from the startup file into 
     *        the registered interrupts table
     */
    inline void default_isr_handler(void) {
        /* get the interrupt number from the system control blocks currently active vector (first 8 bytes) */
        uint8_t isr_number = static_cast<uint8_t>(SCB->ICSR & active_vector_mask);

        /* call the periperals isr_handler with the appropriate type */
        this->isr_table[isr_number].peripheral->irq_handler(isr_table[isr_number].irq_type);
    }

  private:
    InterruptHandler isr_table[static_cast<uint8_t>(InterruptName::floating_point_unit)];
};

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/
extern InterruptManager interrupt_manager;

/****************************** Functions Prototype ************************************/

};  // namespace HAL

#endif /* __HAL_INTERRUPT_H__ */
