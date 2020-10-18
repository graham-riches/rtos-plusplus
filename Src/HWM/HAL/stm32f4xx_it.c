/*! \file stm32f4xx_it.c
*
*  \brief This file contains the ISR routines
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include <board.h>
#include "stm32f4xx_it.h"

/*********************************** Consts ********************************************/


/************************************ Types ********************************************/


/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/


/****************************** Functions Definitions ************************************/


/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler( void )
{

}


/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler( void )
{

  while (1)
  {
      /*
      asm volatile(
                  "tst lr, #4\t\n" // Check EXC_RETURN[2]
                  "ite eq\t\n"
                  "mrseq r0, msp\t\n"
                  "mrsne r0, psp\t\n"
                  "b HardFault_Handler\t\n"
                  : // no output
                  : // no input
                  : "r0" // clobber
                  );
      */
  }
}


/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{

  while (1)
  {

  }
}


/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{

  while (1)
  {

  }
}


/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  while (1)
  {

  }
}


/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{

}


/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{

}


/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{

}


/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
}
