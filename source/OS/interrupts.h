/*! \file interrupts.h
*
*  \brief interrupts module functions and variables declarations.
*
*
*  \author Graham Riches
*/

#ifndef __INTERRUPTS_H__
#define __INTERRUPTS_H__

#ifdef __cplusplus
extern "C"
{
#endif

/****************************** Interrupt Function Definitions ************************************/
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void OTG_FS_IRQHandler(void);
void IRQ_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* __INTERRUPTS_H__ */
