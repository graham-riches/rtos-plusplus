/*! \file scheduler.c
*
*  \brief A simple cooperative scheduler for concurrent tasks.
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "scheduler.h"
#include "stm32f4xx.h"
#include "threading.h"


namespace OS
{

/*********************************** Consts ********************************************/

/************************************ Types ********************************************/

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/

/******************************** Local Variables **************************************/

/****************************** Functions Prototype ************************************/

/****************************** Functions Definition ***********************************/

/**
 * \brief launch the scheduler kernel
 */
__attribute__((naked)) void enter_kernel(void) {
    using namespace OS;
    
    /* disable interrupts */
    __asm("CPSID    I");

    /* load the initial task pointer into R0 */
    __asm("LDR     R0, =system_active_task");

    /* load R2 with the contents of R0 */
    __asm("LDR     R2, [R0]");

    /* copy the saved tasks stack pointer into the CPU */
    __asm("LDR     R4, [R2]");
    __asm("MOV     SP, R4");

    /* Pop registers R0-R11 */
    __asm("POP     {R4-R11}");
    __asm("POP     {R0-R3}");

    /*  Pop the cortex saved context registers */
    __asm("POP     {R4}");
    __asm("MOV     R12, R4");

    /* Skip the saved LR (not valid at startup) */
    __asm("ADD     SP,SP,#4");

    /* load the task function pointer into LR */
    __asm("POP     {R4}");
    __asm("MOV     LR, R4");
    __asm("ADD     SP,SP,#4");

    /* re-enable interrupts and jump to the first task */
    __asm("CPSIE   I ");
    __asm("BX      LR");
}

/**
 * \brief Set the systick timer frequency
 * 
 * \param ticks of the main sysclock per interrupt
 */
void set_systick_frequency(uint32_t ticks) {
    SysTick_Config(ticks);
    NVIC_SetPriority(SysTick_IRQn, 15);
}

};  // namespace OS
