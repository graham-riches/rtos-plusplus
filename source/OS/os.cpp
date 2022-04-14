/*! \file os.cpp
*
*  \brief contains the main kernel functions for running the operating system.
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "os.hpp"
#include "device_port.hpp"
#include "hal_nvic.hpp"
#include "stm32f4xx.h"


namespace os
{
/********************************** Global Objects and Variables *******************************************/
task_control_block* system_active_task;
task_control_block* system_pending_task;

/********************************** Function Definitions *******************************************/

namespace kernel
{
/**
 * \brief Initialize the kernel
 */
void setup(void) {
    DISABLE_INTERRUPTS();

    // Initialize the task pointers to initialize the kernel
    system_active_task = scheduler::get_active_task_control_block();
    system_pending_task = scheduler::get_pending_task_control_block();

    // Activate the first task
    system_active_task->thread_ptr->set_status(thread::status::active);

    // Setup core interrupt priorities
    HAL::nvic::set_priority(stm32f4_irq::systick_handler, HAL::nvic::isr_preemption_priority::level_16);
    HAL::nvic::set_priority(stm32f4_irq::pendsv_handler, HAL::nvic::isr_preemption_priority::level_16);

    ENABLE_INTERRUPTS();
}

/**
 * \brief pick up the first task control block pointer
 */
// clang-format off
__attribute__((naked))  void enter(void) {
    using namespace os;

    __asm(
        "CPSID      I                        \n" // Disable interrupts
        "LDR        R0, =system_active_task  \n" // Load the active task pointer into r0
        "LDR        R1, [R0]                 \n" // Load the stack pointer from the contents of task into R1
        "LDR        R4, [R1]                 \n" // Copy the saved stack pointer into R4
        "MOV        SP, R4                   \n" // Update the stack pointer
        "POP        {R4-R11}                 \n" // Pop R4-R11 off the stack
        "VPOP       {D0-D15}                 \n" //             
        "POP        {R0}                     \n" //
        "VMSR       fpscr, r0                \n" //       
        "POP        {R0, R1, R2, R3, R12}    \n" // Pop R0-R3 and R12 off the stack
        "ADD        SP,SP,#4                 \n" // Skip over the saved LR as it is invalid on startup
        "POP        {R4}                     \n" // Grab the task function pointer
        "MOV        LR, R4                   \n" // Restore the LR state
        "ADD        SP,SP,#4                 \n" // Increment the stack pointer
        "CPSIE      I                        \n" // Re-enable interrupts
        "BX         LR                       \n" // Branch to the link register
    );
}
// clang-format on

/**
 * \brief Set the systick timer frequency
 * 
 * \param ticks of the main sysclock per interrupt
 */
void set_systick_frequency(uint32_t ticks) {
    SysTick_Config(ticks);
}

};  // namespace kernel

};  // namespace os
