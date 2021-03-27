/*! \file hal_interrupt.cpp
*
*  \brief abstract base class for interrupt managemente.
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "hal_interrupt.h"
#include "stm32f4xx.h"

namespace HAL
{

/*********************************** Consts ********************************************/
constexpr uint32_t active_vector_mask = 0xFF;                                                        //!< mask for currently active interrupt vector
constexpr uint8_t external_interrupt_offset = static_cast<uint8_t>(InterruptName::window_watchdog);  //!< offset for external interrupts

/************************************ Types ********************************************/

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/
InterruptManager interrupt_manager;

/******************************** Local Variables **************************************/

/****************************** Functions Prototype ************************************/

/****************************** Functions Definition ***********************************/
/**
 * \brief Construct a new Interrupt Manager:: Interrupt Manager object
 * \note this will configure the microcontroller NVIC priority grouping
 * 
 */
InterruptManager::InterruptManager() {
    NVIC_SetPriorityGrouping(HAL_NVIC_PRIORITY_GROUP_LEVEL);
}

/**
 * \brief set the priority grouping and subpriority for an interrupt
 * 
 * \param interrupt the interrupt to set the priority for
 * \param preemption_priority premption priority level
 */
void InterruptManager::set_priority(InterruptName interrupt, PreemptionPriority preemption_priority) {
    uint32_t priority = NVIC_EncodePriority(HAL_NVIC_PRIORITY_GROUP_LEVEL, static_cast<uint32_t>(preemption_priority), 1UL);
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
void InterruptManager::register_callback(InterruptName interrupt, 
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
 * 
 */
void InterruptManager::default_isr_handler(void) {
    /* get the interrupt number from the system control blocks currently active vector (first 8 bytes) */
    uint8_t isr_number = static_cast<uint8_t>(SCB->ICSR & active_vector_mask);

    /* call the periperals isr_handler with the appropriate type */
    this->isr_table[isr_number].peripheral->irq_handler(isr_table[isr_number].irq_type);
}

};  // namespace HAL

/****************************** Vector Table Function Pointer Definitions ***********************************/
extern "C"
{
    /**
    * \brief this single function remaps all vector table interrups to the C++ interrupt handler
    *        which calls the appropriate IRQ which is generally registered to a specific object.
    */
    void IRQHandler(void) {
        HAL::interrupt_manager.default_isr_handler();
    }
}
