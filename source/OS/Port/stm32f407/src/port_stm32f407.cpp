// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Graham Riches

#include "port_stm32f407.hpp"
#include "interrupt_lock_guard.hpp"
#include "os.hpp"
#include "stm32f4xx.h"
#include "thread.hpp"

//!< Extern variables exposed in linker script
extern uint32_t _sidata;  // Start of data segment in flash
extern uint32_t _sdata;   // Start address for the .data section. defined in linker script
extern uint32_t _edata;   // End address for the .data section. defined in linker script
extern uint32_t _sbss;    // Start address for the .bss section. defined in linker script
extern uint32_t _ebss;    // End address for the .bss section. defined in linker script

// Stack context saved during a fault
#pragma pack(push, 0)
struct StackContext {
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr;
    uint32_t return_address;
    uint32_t xpsr;
};
#pragma pack(pop)

// Functions called from Linker script/startup code
extern "C" void __libc_init_array();
extern "C" void __system_startup();
extern "C" void fault_handler(StackContext* context);
extern "C" int main();



// clang-format off
#define HALT_IF_DEBUGGING()                                   \
    do {                                                      \
        if ( (*(volatile uint32_t*)0xE000EDF0) & (1 << 0) ) { \
            __asm("bkpt 1");                                  \
        }                                                     \
    } while ( 0 )

#define HANDLE_FAULT()                        \
    do {                                      \
        __asm volatile("tst lr, #4      \n"   \
                       "ite eq          \n"   \
                       "mrseq r0, msp   \n"   \
                       "mrsne r0, psp   \n"   \
                       "b fault_handler \n"); \
    } while( 0 )
// clang-format on

void set_pending_context_switch() {
    os::system_pending_task = os::scheduler::get_pending_task_control_block();
    SCB->ICSR = SCB->ICSR | SCB_ICSR_PENDSVSET_Msk;
}

bool is_context_switch_pending() {
    return static_cast<bool>(SCB->ICSR & SCB_ICSR_PENDSVSET_Msk);
}

void bootstrap_device_port() {
    // Manually set device specific interrupt priorities here
    // Set both SysTick and PendSv to lowest priority

    uint32_t priority = NVIC_EncodePriority(0, 15, 1UL);    
    NVIC_SetPriority(SysTick_IRQn, priority);
    NVIC_EnableIRQ(SysTick_IRQn);    
    NVIC_SetPriority(PendSV_IRQn, priority);
    NVIC_EnableIRQ(PendSV_IRQn);
}

void isr_default_handler() {
    while ( 1 ) {
        HALT_IF_DEBUGGING();
    }
}

/**
 * \brief Reset handler to start up the processor
 *  - initializes stack
 *  - initializes heap
 *  - copy data segment from flash to ram
 *  - zero .bss
 *  - libc init
 *  - system init
 *  - main
 */
extern "C" void __attribute__((naked)) isr_reset_handler() {
    uint32_t* p_dest;
    uint32_t* p_src;

    // Copy data segment from flash to RAM
    p_src = &_sidata;
    for ( p_dest = &_sdata; p_dest < &_edata; ) {
        *p_dest++ = *p_src++;
    }

    // Zero .bss
    for ( p_dest = &_sbss; p_dest < &_ebss; ) {
        *p_dest++ = 0;
    }

    // Init libc
    __libc_init_array();

    // MCU startup
    __system_startup();

    // Jump to main
    main();
}

/**
 * \brief This function handles Non-maskable interrupt
 */
void isr_nmi_handler() { }

/**
 * \brief This function handles Hard fault interrupt
 */
void isr_hard_fault_handler() {
    __asm("bkpt 1");
    HANDLE_FAULT();
}

/**
 * \brief This function handles Memory management fault
 */
void isr_mem_manager_handler() {
    __asm("bkpt 1");
    HANDLE_FAULT();
}

/**
 * \brief This function handles Pre-fetch fault, memory access fault
 */
void isr_bus_fault_handler() {
    __asm("bkpt 1");
    HANDLE_FAULT();
}

/**
 * \brief This function handles Undefined instruction or illegal state
 */
void isr_usage_fault_handler() {
    __asm("bkpt 1");
    HANDLE_FAULT();
}

/**
 * \brief This function handles System service call via SWI instruction
 */
void isr_svc_handler() { }

/**
 * \brief This function handles Debug monitor
 */
void isr_debug_mon_handler() { }

/**
 * \brief Handling threading context switches which are triggered by the scheduler as pending
 *        system calls. This allows the SysTick interrupt to run at a high priority while the 
 *        context switching can be handled at a lower level.
 */
// clang-format off
__attribute__((naked)) void isr_pend_sv_handler() {
    using namespace os;

    __asm("CPSID      I                        \n"  // Disable interrupts
          "PUSH       {R4-R11}                 \n"  // Push the remaining core registers
          "VPUSH      {D0-D15}                 \n"  // Push floating point context
          "VMRS       R0,fpscr                 \n"  // Get FPU status/control register
          "PUSH       {R0}                     \n"  // Push floating point control register
          "LDR        R0, =system_active_task  \n"  // Load the active task pointer into
          "LDR        R1, [R0]                 \n"  // Dereference the pointer
          "MOV        R4, SP                   \n"  // Stash the current stack pointer
          "STR        R4, [R1]                 \n"  // Update the pointer to the current thread task with the current stack pointer
          "LDR        R1, =system_pending_task \n"  // Get the next task pointer
          "LDR        R2, [R1]                 \n"  // Dereference the pointer
          "STR        R2, [R0]                 \n"  // Update the active thread to be the pending thread
          "LDR        R4, [R2]                 \n"  // Get the new stack pointer by dereferencing the original pointer
          "MOV        SP, R4                   \n"  // Push it to the CPU stack pointer register
          "VPOP       {D0-D15}                 \n"  // Restore floating point context
          "POP        {R0}                     \n"  // Pop floating point status/control register
          "VMSR       fpscr, R0                \n"  // Restore floating point control register
          "POP        {R4-R11}                 \n"  // Pop the stored registers
          "CPSIE      I                        \n"  // Re-enable interrupts
          "BX         LR                       \n"  // Return
    );
}
// clang-format on

/**
 * \brief Use the SysTick interrupt to drive the operating system core clock and scheduler.
 *        The scheduler will raise a PendSV interrupt flag if any thread context switches are 
 *        required.
 */
void isr_systick_handler() {
    os::interrupt_guard guard;
    os::scheduler::update_system_ticks(1);
    os::scheduler::update();
}

/**
 * \brief custom hardfault handler to save the register state
 * 
 * \param context register context
 * \note This should not be optimized!!
 */
__attribute__((optimize("O0"))) void fault_handler(StackContext* context) {
    (void)context;
    HALT_IF_DEBUGGING();
}

/****************************** Vector Table ************************************/
// Initial stack top that is the first entry in the vector table. This is set in the linker script to the
// top of the user stack space
extern "C" void _estack();

// Definition of an IRQ handler type
using isr_handler_function = void (*)();

// Device Vector Table
__attribute__((used, section(".vectors"))) isr_handler_function vectors[STM32F4_TOTAL_ISR + 1] = {
    // Initial stack pointer
    &_estack,

    // Cortext M4 Interrupts
    isr_reset_handler,
    isr_nmi_handler,
    isr_hard_fault_handler,
    isr_mem_manager_handler,
    isr_bus_fault_handler,
    isr_usage_fault_handler,
    0,
    0,
    0,
    0,
    isr_svc_handler,
    isr_debug_mon_handler,
    0,
    isr_pend_sv_handler,
    isr_systick_handler,

    // STM32 Peripheral Interrupts
    isr_default_handler,  // window_watchdog
    isr_default_handler,  // power_voltage_detection
    isr_default_handler,  // time_stamp
    isr_default_handler,  // rtc_wakeup
    isr_default_handler,  // flash
    isr_default_handler,  // rcc
    isr_default_handler,  // exti_0
    isr_default_handler,  // exti_1
    isr_default_handler,  // exti_2
    isr_default_handler,  // exti_3
    isr_default_handler,  // exti_4
    isr_default_handler,  // dma_1_stream_0
    isr_default_handler,  // dma_1_stream_1
    isr_default_handler,  // dma_1_stream_2
    isr_default_handler,  // dma_1_stream_3
    isr_default_handler,  // dma_1_stream_4
    isr_default_handler,  // dma_1_stream_5
    isr_default_handler,  // dma_1_stream_6
    isr_default_handler,  // adc
    isr_default_handler,  // can_1_tx
    isr_default_handler,  // can_1_rx_0
    isr_default_handler,  // can_1_rx_1
    isr_default_handler,  // can_1_sce
    isr_default_handler,  // exti_9_5
    isr_default_handler,  // timer_1_break
    isr_default_handler,  // timer_1_update
    isr_default_handler,  // timer_1_trigger_commutation
    isr_default_handler,  // timer_1_capture_compare
    isr_default_handler,  // timer_2
    isr_default_handler,  // timer_3
    isr_default_handler,  // timer_4
    isr_default_handler,  // i2c_1_event
    isr_default_handler,  // i2c_1_error
    isr_default_handler,  // i2c_2_event
    isr_default_handler,  // i2c_2_error
    isr_default_handler,  // spi_1
    isr_default_handler,  // spi_2
    isr_default_handler,  // usart_1
    isr_default_handler,  // usart_2
    isr_usart3_handler,   // usart_3
    isr_default_handler,  // exti_15_10
    isr_default_handler,  // rtc_alarm
    isr_default_handler,  // otg_fs_wakeup
    isr_default_handler,  // timer_8_break
    isr_default_handler,  // timer_8_update
    isr_default_handler,  // timer_8_trigger_commutation
    isr_default_handler,  // timer_8_capture_compare
    isr_default_handler,  // dma_1_stream_7
    isr_default_handler,  // fsmc
    isr_default_handler,  // sdio
    isr_default_handler,  // timer_5
    isr_default_handler,  // spi_3
    isr_default_handler,  // uart_4
    isr_default_handler,  // uart_5
    isr_default_handler,  // timer_6_dac
    isr_default_handler,  // timer_7
    isr_default_handler,  // dma_2_stream_0
    isr_default_handler,  // dma_2_stream_1
    isr_default_handler,  // dma_2_stream_2
    isr_default_handler,  // dma_2_stream_3
    isr_default_handler,  // dma_2_stream_4
    isr_default_handler,  // ethernet
    isr_default_handler,  // ethernet_wakeup
    isr_default_handler,  // can_2_tx
    isr_default_handler,  // can_2_rx_0
    isr_default_handler,  // can_2_rx_1
    isr_default_handler,  // can_2_sce
    isr_default_handler,  // otg_fs
    isr_default_handler,  // dma_2_stream_5
    isr_default_handler,  // dma_2_stream_6
    isr_default_handler,  // dma_2_stream_7
    isr_default_handler,  // usart_6
    isr_default_handler,  // i2c_3_event
    isr_default_handler,  // i2c_3_error
    isr_default_handler,  // otg_hs_endpoint_1_out
    isr_default_handler,  // otg_hs_endpoint_1_in
    isr_default_handler,  // otg_hs_wakeup
    isr_default_handler,  // otg_hs
    isr_default_handler,  // dcmi
    isr_default_handler,  // cryptography_unit
    isr_default_handler,  // hash_random_number
    isr_default_handler,  // floating_point_unit
};