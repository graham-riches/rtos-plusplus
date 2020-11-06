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
#include "threading.h"
#include "timer.h"

/*********************************** Consts ********************************************/

/************************************ Types ********************************************/
/**
 * \brief struct for storing stack context during a crash
 */
#pragma pack( 0 )
typedef struct
{
   uint32_t r0;
   uint32_t r1;
   uint32_t r2;
   uint32_t r3;
   uint32_t r12;
   uint32_t lr;
   uint32_t return_address;
   uint32_t xpsr;
} StackContext_t;
#pragma pack( 1 )

/*********************************** Macros ********************************************/
#define HALT_IF_DEBUGGING( )                                   \
   do                                                          \
   {                                                           \
      if ( ( *(volatile uint32_t *)0xE000EDF0 ) & ( 1 << 0 ) ) \
      {                                                        \
         __asm( "bkpt 1" );                                    \
      }                                                        \
   } while ( 0 )

/******************************* Global Variables **************************************/

/******************************* Local Variables **************************************/



/****************************** Functions Declarations ************************************/
extern "C"{

void fault_handler( StackContext_t *context );

}


/****************************** Functions Definitions - Includes Cortex M4 Interrupts ************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler( void ) { }


/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler( void )
{
   while ( 1 )
   {
      __asm volatile( "tst lr, #4 \n"
                      "ite eq \n"
                      "mrseq r0, msp \n"
                      "mrsne r0, psp \n"
                      "b fault_handler \n" );
   }
}


/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler( void )
{
   while ( 1 )
   { }
}


/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler( void )
{
   while ( 1 )
   { }
}


/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler( void )
{
   while ( 1 )
   { }
}


/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler( void ) { }


/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler( void ) { }


/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler( void ) { }


/**
  * @brief This function handles System tick timer.
  */
__attribute__((naked)) void SysTick_Handler( void ) 
{   
   /* use the OS namespace to access the task control block pointer */
   using namespace OS;

   /* increment the tick counter */
   system_ticks++;

   /* run the task context switching from the scheduler */
   __asm( 
      "CPSID      I                        \n" /* disable interrupts */
      "PUSH       {R4-R11}                 \n" /* push the remaining core registers */
      "LDR        R0, =system_active_task  \n" /* load the active task pointer into r0*/
      "LDR        R1, [R0]                 \n" /* load the stack pointer from the contents of task into R1 */
      "MOV        R4, SP                   \n" /* move the CPU stack pointer to R4 */
      "STR        R4, [R1]                 \n" /* store the stack pointer into task */
      "LDR        R1, [R1, #4]             \n" /* get the next task pointer and load it into R1 - 4 byte offset from stack pointer to next task */
      "STR        R1, [R0]                 \n" /* store the contents of R1 in R0 */
      "LDR        R4, [R1]                 \n" /* get the new stack pointer */
      "MOV        SP, R4                   \n" /* push it to the CPU stack pointer register */
      "POP        {R4-R11}                 \n" /* pop the stored registers */
      "CPSIE      I                        \n" /* re-enable interrupts */
      "BX         LR                       \n" /* branch to the link register */
   );
}


/**
 * \brief custom hardfault handler to save the register state
 * 
 * \param context register context
 * \note this should not be optimized ***
 */
void fault_handler( StackContext_t *context )
{
   PARAMETER_NOT_USED( context );
   HALT_IF_DEBUGGING( );
}
