/*! \file interrupts.c
*
*  \brief Operating system level interrupts.
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "interrupts.h"
#include "board.h"
#include "common.h"
#include "thread_impl.h"
#include "os.h"
#include "hal_interrupt.h"
#include "cm4_port.h"

/************************************ Types ********************************************/
/**
 * \brief struct for storing stack context during a crash
 */
#pragma pack(0)
typedef struct {
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr;
    uint32_t return_address;
    uint32_t xpsr;
} StackContext_t;
#pragma pack(1)

/*********************************** Macros ********************************************/
#define HALT_IF_DEBUGGING()                                   \
    do {                                                      \
        if ( (*(volatile uint32_t*)0xE000EDF0) & (1 << 0) ) { \
            __asm("bkpt 1");                                  \
        }                                                     \
    } while ( 0 )


/****************************** Function Declarations ************************************/
extern "C"
{
    void fault_handler(StackContext_t* context);
}

/****************************** Functions Definitions - Includes Cortex M4 Interrupts ************************************/
/**
 * \brief This function handles Non maskable interrupt
 */
void NMI_Handler(void) { }

/**
 * \brief This function handles Hard fault interrupt.
 */
void HardFault_Handler(void) {
    while ( 1 ) {
        __asm volatile("tst lr, #4      \n"
                       "ite eq          \n"
                       "mrseq r0, msp   \n"
                       "mrsne r0, psp   \n"
                       "b fault_handler \n");
    }
}

/**
 * \brief This function handles Memory management fault.
 */
void MemManage_Handler(void) {
    while ( 1 ) {
    }
}

/**
 * \brief This function handles Pre-fetch fault, memory access fault.
 */
void BusFault_Handler(void) {
    while ( 1 ) {
    }
}

/**
 * \brief This function handles Undefined instruction or illegal state.
 */
void UsageFault_Handler(void) {
    while ( 1 ) {
    }
}

/**
 * \brief This function handles System service call via SWI instruction.
 */
void SVC_Handler(void) { }

/**
 * \brief This function handles Debug monitor.
 */
void DebugMon_Handler(void) { }

/**
 * \brief Handling threading context switches which are triggered by the scheduler as pending
 *        system calls. This allows the SysTick interrupt to run at a high priority while the 
 *        context switching can be handled at a lower level.
 */
__attribute__((naked)) void PendSV_Handler(void) {   
    using namespace os;

    __asm(
        "CPSID      I                        \n" /* disable interrupts */
        "PUSH       {R4-R11}                 \n" /* push the remaining core registers */
        "LDR        R0, =system_active_task  \n" /* load the active task pointer into */
        "LDR        R1, [R0]                 \n" /* dereference the pointer */
        "MOV        R4, SP                   \n" /* stash the current stack pointer */
        "STR        R4, [R1]                 \n" /* update the pointer to the current thread task with the current stack pointer */
        "LDR        R1, =system_pending_task \n" /* get the next task pointer */
        "LDR        R2, [R1]                 \n" /* dereference the pointer */
        "STR        R2, [R0]                 \n" /* update the active thread to be the pending thread */        
        "LDR        R4, [R2]                 \n" /* get the new stack pointer by dereferencing the original pointer */
        "MOV        SP, R4                   \n" /* push it to the CPU stack pointer register */
        "POP        {R4-R11}                 \n" /* pop the stored registers */
        "CPSIE      I                        \n" /* re-enable interrupts */
        "BX         LR                       \n" /* return */
    );
}

/**
 * \brief Use the SysTick interrupt to drive the operating system core clock and scheduler.
 *        The scheduler will raise a PendSV interrupt flag if any thread context switches are 
 *        required.
 */
void SysTick_Handler(void) {
    DISABLE_INTERRUPTS();
    os::system_clock::update_sytem_ticks(1);
    os::scheduler::update();
    ENABLE_INTERRUPTS();  
}

/**
* \brief this single function remaps all vector table interrups to the C++ interrupt handler object
*        which calls the appropriate IRQ which is generally registered to a specific object.
*/
void IRQHandler(void) {
    HAL::interrupt_manager.default_isr_handler();
}

/**
 * \brief custom hardfault handler to save the register state
 * 
 * \param context register context
 * \note this should not be optimized ***
 */
__attribute__((optimize("O0"))) void fault_handler(StackContext_t* context) {
    PARAMETER_NOT_USED(context);
    HALT_IF_DEBUGGING();
}
