/**
  ******************************************************************************
  * @file      startup_stm32f407xx.s
  * @author    MCD Application Team
  * @brief     STM32F407xx Devices vector table for GCC based toolchains. 
  *            This module performs:
  *                - Set the initial SP
  *                - Set the initial PC == Reset_Handler,
  *                - Set the vector table entries with the exceptions ISR address
  *                - Branches to main in the C library (which eventually
  *                  calls main()).
  *            After Reset the Cortex-M4 processor is in Thread mode,
  *            priority is Privileged, and the Stack is set to Main.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
    
  .syntax unified
  .cpu cortex-m4
  .fpu softvfp
  .thumb

.global  g_pfnVectors
.global  Default_Handler

/* start address for the initialization values of the .data section. 
defined in linker script */
.word  _sidata
/* start address for the .data section. defined in linker script */  
.word  _sdata
/* end address for the .data section. defined in linker script */
.word  _edata
/* start address for the .bss section. defined in linker script */
.word  _sbss
/* end address for the .bss section. defined in linker script */
.word  _ebss
/* stack used for SystemInit_ExtMemCtl; always internal RAM used */

/**
 * @brief  This is the code that gets called when the processor first
 *          starts execution following a reset event. Only the absolutely
 *          necessary set is performed, after which the application
 *          supplied main() routine is called. 
 * @param  None
 * @retval : None
*/

    .section  .text.Reset_Handler
  .weak  Reset_Handler
  .type  Reset_Handler, %function
Reset_Handler:  
  ldr   sp, =_estack     /* set stack pointer */

/* Copy the data segment initializers from flash to SRAM */  
  movs  r1, #0
  b  LoopCopyDataInit

CopyDataInit:
  ldr  r3, =_sidata
  ldr  r3, [r3, r1]
  str  r3, [r0, r1]
  adds  r1, r1, #4
    
LoopCopyDataInit:
  ldr  r0, =_sdata
  ldr  r3, =_edata
  adds  r2, r0, r1
  cmp  r2, r3
  bcc  CopyDataInit
  ldr  r2, =_sbss
  b  LoopFillZerobss
/* Zero fill the bss segment. */  
FillZerobss:
  movs  r3, #0
  str  r3, [r2], #4
    
LoopFillZerobss:
  ldr  r3, = _ebss
  cmp  r2, r3
  bcc  FillZerobss

/* Call the clock system intitialization function.*/
/*  bl  SystemInit   */
/* Call static constructors */
   bl __libc_init_array
/* Call the application's entry point.*/
  bl  main
  bx  lr    
.size  Reset_Handler, .-Reset_Handler

/**
 * @brief  This is the code that gets called when the processor receives an 
 *         unexpected interrupt.  This simply enters an infinite loop, preserving
 *         the system state for examination by a debugger.
 * @param  None     
 * @retval None       
*/
    .section  .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
  b  Infinite_Loop
  .size  Default_Handler, .-Default_Handler
/******************************************************************************
*
* The minimal vector table for a Cortex M3. Note that the proper constructs
* must be placed on this to ensure that it ends up at physical address
* 0x0000.0000.
* 
*******************************************************************************/
   .section  .isr_vector,"a",%progbits
  .type  g_pfnVectors, %object
  .size  g_pfnVectors, .-g_pfnVectors
    
    
g_pfnVectors:
  .word  _estack
  .word  Reset_Handler
  .word  NMI_Handler
  .word  HardFault_Handler
  .word  MemManage_Handler
  .word  BusFault_Handler
  .word  UsageFault_Handler
  .word  0
  .word  0
  .word  0
  .word  0
  .word  SVC_Handler
  .word  DebugMon_Handler
  .word  0
  .word  PendSV_Handler
  .word  SysTick_Handler
  
  /* External Interrupts */
  .word     IRQHandler           /* Window WatchDog              */                                        
  .word     IRQHandler           /* PVD through EXTI Line detection */                        
  .word     IRQHandler           /* Tamper and TimeStamps through the EXTI line */            
  .word     IRQHandler           /* RTC Wakeup through the EXTI line */                      
  .word     IRQHandler           /* FLASH                        */                                          
  .word     IRQHandler           /* RCC                          */                                            
  .word     IRQHandler           /* EXTI Line0                   */                        
  .word     IRQHandler           /* EXTI Line1                   */                          
  .word     IRQHandler           /* EXTI Line2                   */                          
  .word     IRQHandler           /* EXTI Line3                   */                          
  .word     IRQHandler           /* EXTI Line4                   */                          
  .word     IRQHandler           /* DMA1 Stream 0                */                  
  .word     IRQHandler           /* DMA1 Stream 1                */                   
  .word     IRQHandler           /* DMA1 Stream 2                */                   
  .word     IRQHandler           /* DMA1 Stream 3                */                   
  .word     IRQHandler           /* DMA1 Stream 4                */                   
  .word     IRQHandler           /* DMA1 Stream 5                */                   
  .word     IRQHandler           /* DMA1 Stream 6                */                   
  .word     IRQHandler           /* ADC1, ADC2 and ADC3s         */                   
  .word     IRQHandler           /* CAN1 TX                      */                         
  .word     IRQHandler           /* CAN1 RX0                     */                          
  .word     IRQHandler           /* CAN1 RX1                     */                          
  .word     IRQHandler           /* CAN1 SCE                     */                          
  .word     IRQHandler           /* External Line[9:5]s          */                          
  .word     IRQHandler           /* TIM1 Break and TIM9          */         
  .word     IRQHandler           /* TIM1 Update and TIM10        */         
  .word     IRQHandler           /* TIM1 Trigger and Commutation and TIM11 */
  .word     IRQHandler           /* TIM1 Capture Compare         */                          
  .word     IRQHandler           /* TIM2                         */                   
  .word     IRQHandler           /* TIM3                         */                   
  .word     IRQHandler           /* TIM4                         */                   
  .word     IRQHandler           /* I2C1 Event                   */                          
  .word     IRQHandler           /* I2C1 Error                   */                          
  .word     IRQHandler           /* I2C2 Event                   */                          
  .word     IRQHandler           /* I2C2 Error                   */                            
  .word     IRQHandler           /* SPI1                         */                   
  .word     IRQHandler           /* SPI2                         */                   
  .word     IRQHandler           /* USART1                       */                   
  .word     IRQHandler           /* USART2                       */                   
  .word     IRQHandler           /* USART3                       */                   
  .word     IRQHandler           /* External Line[15:10]s        */                          
  .word     IRQHandler           /* RTC Alarm (A and B) through EXTI Line */                 
  .word     IRQHandler           /* USB OTG FS Wakeup through EXTI line */                       
  .word     IRQHandler           /* TIM8 Break and TIM12         */         
  .word     IRQHandler           /* TIM8 Update and TIM13        */         
  .word     IRQHandler           /* TIM8 Trigger and Commutation and TIM14 */
  .word     IRQHandler           /* TIM8 Capture Compare         */                          
  .word     IRQHandler           /* DMA1 Stream7                 */                          
  .word     IRQHandler           /* FSMC                         */                   
  .word     IRQHandler           /* SDIO                         */                   
  .word     IRQHandler           /* TIM5                         */                   
  .word     IRQHandler           /* SPI3                         */                   
  .word     IRQHandler           /* UART4                        */                   
  .word     IRQHandler           /* UART5                        */                   
  .word     IRQHandler           /* TIM6 and DAC1&2 underrun errors */                   
  .word     IRQHandler           /* TIM7                         */
  .word     IRQHandler           /* DMA2 Stream 0                */                   
  .word     IRQHandler           /* DMA2 Stream 1                */                   
  .word     IRQHandler           /* DMA2 Stream 2                */                   
  .word     IRQHandler           /* DMA2 Stream 3                */                   
  .word     IRQHandler           /* DMA2 Stream 4                */                   
  .word     IRQHandler           /* Ethernet                     */                   
  .word     IRQHandler           /* Ethernet Wakeup through EXTI line */                     
  .word     IRQHandler           /* CAN2 TX                      */                          
  .word     IRQHandler           /* CAN2 RX0                     */                          
  .word     IRQHandler           /* CAN2 RX1                     */                          
  .word     IRQHandler           /* CAN2 SCE                     */                          
  .word     IRQHandler           /* USB OTG FS                   */                   
  .word     IRQHandler           /* DMA2 Stream 5                */                   
  .word     IRQHandler           /* DMA2 Stream 6                */                   
  .word     IRQHandler           /* DMA2 Stream 7                */                   
  .word     IRQHandler           /* USART6                       */                    
  .word     IRQHandler           /* I2C3 event                   */                          
  .word     IRQHandler           /* I2C3 error                   */                          
  .word     IRQHandler           /* USB OTG HS End Point 1 Out   */                   
  .word     IRQHandler           /* USB OTG HS End Point 1 In    */                   
  .word     IRQHandler           /* USB OTG HS Wakeup through EXTI */                         
  .word     IRQHandler           /* USB OTG HS                   */                   
  .word     IRQHandler           /* DCMI                         */                   
  .word     0                    /* CRYP crypto                  */                   
  .word     IRQHandler           /* Hash and Rng                 */
  .word     IRQHandler           /* FPU                          */
                         
                         
/*******************************************************************************
*
* Provide weak aliases for each Exception handler to the Default_Handler. 
* As they are weak aliases, any function with the same name will override 
* this definition.
* 
*******************************************************************************/
   .weak      NMI_Handler
   .thumb_set NMI_Handler,Default_Handler
  
   .weak      HardFault_Handler
   .thumb_set HardFault_Handler,Default_Handler
  
   .weak      MemManage_Handler
   .thumb_set MemManage_Handler,Default_Handler
  
   .weak      BusFault_Handler
   .thumb_set BusFault_Handler,Default_Handler

   .weak      UsageFault_Handler
   .thumb_set UsageFault_Handler,Default_Handler

   .weak      SVC_Handler
   .thumb_set SVC_Handler,Default_Handler

   .weak      DebugMon_Handler
   .thumb_set DebugMon_Handler,Default_Handler

   .weak      PendSV_Handler
   .thumb_set PendSV_Handler,Default_Handler

   .weak      SysTick_Handler
   .thumb_set SysTick_Handler,Default_Handler              
  
   .weak      IRQHandler                   
   .thumb_set IRQHandler,Default_Handler      
   

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
