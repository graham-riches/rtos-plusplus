/*! \file scheduler.c
*
*  \brief A simple cooperative scheduler for concurrent tasks.
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "scheduler.h"
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
__attribute__((naked)) void enter_kernel( void )
{   
   /* load the initial task pointer ino R0 */
   __asm("LDR     R0, =system_active_task");

   /* load R2 with the contents of R0 */
   __asm("LDR     R2, [R0]");

   /* copy the saved tasks stack pointer into the CPU */
   __asm("LDR     R4, [R2]");
   __asm("MOV     SP, R4");

   /* Pop registers R8-R11 */
   __asm("POP     {R4-R7}");
   __asm("MOV     R8, R4");
   __asm("MOV     R9, R5");
   __asm("MOV     R10, R6");
   __asm("MOV     R11, R7");

   /* Pop registers R4-R7 */
   __asm("POP     {R4-R7}");

   /*  Pop the cortex saved context registers */
   __asm("POP     {R0-R3}");
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

};  // namespace OS
