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
#include "core_cm4.h"

namespace HAL
{
namespace Interrupt
{
/*********************************** Consts ********************************************/
constexpr uint8_t isr_table_size = static_cast<uint8_t>( InterruptName::floating_point_unit );        //!< total number of system interrupts
constexpr uint32_t active_vector_mask = 0xFF;                                                         //!< mask for currently active interrupt vector
constexpr uint8_t external_interrupt_offset = static_cast<uint8_t>( InterruptName::window_watchdog ); //!< offset for external interrupts

/************************************ Types ********************************************/

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/

/******************************** Local Variables **************************************/
static InterruptHandler isr_table[isr_table_size];  //!< array of interrupt handlers

/****************************** Functions Prototype ************************************/
static void default_isr_handler( void );

/****************************** Functions Definition ***********************************/
/**
 * \brief register an interrupt service routing from a peripheral to an instance
 *        in the ISR handler table
 * 
 * \param interrupt the interrupt to register
 * \param peripheral pointer to the derived peripheral with interrupt capabilities
 * \param type type of the interrupt (for peripherals with multiple interrupts)
 * \param priority NVIC priorits
 */
void register_callback( InterruptName interrupt, InterruptPeripheral *peripheral, uint8_t type, uint32_t priority )
{
   /* fill the handler structure */
   InterruptHandler handler;
   handler.peripheral = peripheral;
   handler.irq_type = type;

   /* place it in the handler array */
   isr_table[static_cast<uint8_t>( interrupt )] = handler;

   /* enable it in the NVIC */
   IRQn external_interrupt_number = static_cast<IRQn>( static_cast<uint8_t>( interrupt ) - external_interrupt_offset );
   NVIC_SetPriority( external_interrupt_number, priority );
   NVIC_EnableIRQ( external_interrupt_number );
}

/**
 * \brief Set the systick timer frequency
 * 
 * \param ticks of the main sysclock per interrupt
 */
void set_systick_frequency( uint32_t ticks )
{
   SysTick_Config( ticks );
}

/**
 * \brief default ISR handler to map specific function pointers from the startup file into 
 *        the registered interrupts table
 * 
 */
static void default_isr_handler( void )
{
   /* get the interrupt number from the system control blocks currently active vector (first 8 bytes) */
   uint8_t isr_number = static_cast<uint8_t>( SCB->ICSR & active_vector_mask );

   /* call the periperals isr_handler with the appropriate type */
   isr_table[isr_number].peripheral->irq_handler( isr_table[isr_number].irq_type );
}

};  // namespace Interrupt
};  // namespace HAL


/****************************** Vector Table Function Pointer Definitions ***********************************/
/*
   This section contains all the function pointer definitions that are defined in the vector table in the 
   startup assembly file. These functions just redirect the output to the appropriate registered handler. This
   is unfortunately required in order for the NVIC to maintain priority levels, etc.
*/
extern "C"
{
   void USART2_IRQHandler( void )
   {
      HAL::Interrupt::default_isr_handler( );
   }
}