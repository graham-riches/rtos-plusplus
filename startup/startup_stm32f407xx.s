  .syntax unified
  .cpu cortex-m4
  .fpu softvfp
  .thumb

// Start address for the initialization values of the .data section. defined in linker script
.word  _sidata
// Start address for the .data section. defined in linker script
.word  _sdata
// End address for the .data section. defined in linker script
.word  _edata
// Start address for the .bss section. defined in linker script
.word  _sbss
// End address for the .bss section. defined in linker script
.word  _ebss
// stack used for SystemInit_ExtMemCtl; always internal RAM used


// System Startup Code
.section  .text.isr_reset_handler
  .weak  isr_reset_handler
  .type  isr_reset_handler, %function
isr_reset_handler:  
  ldr   sp, =_estack     // Initialize Stack Pointer

// Copy the data segment initializers from flash to SRAM 
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

// Zero fill BSS
FillZerobss:
  movs  r3, #0
  str  r3, [r2], #4
    
LoopFillZerobss:
  ldr  r3, = _ebss
  cmp  r2, r3
  bcc  FillZerobss

// Call the HAL startup code
    bl __system_startup

// Call static constructors
   bl __libc_init_array

// Call the application's entry point
  bl  main
  bx  lr    
.size  isr_reset_handler, .-isr_reset_handler
