# RTOS-PlusPlus
---
# Summary
A Real-Time Operating System (RTOS) and Hardware Abstraction Layer (HAL) for STM32 microcontrollers (currently only supporting F4) written in modern C++ (C++17). This is currently a personal project that I undertook with the goal of learning more about the low-level details of Real-Time Operating Systems and direct register level implementations of hardware peripherals without relying on a vendor supplied HAL layer.


# Contents
>- [HAL Overview](https://github.com/graham-riches/rtos-plusplus#hal-overview)
>- [RTOS Overview](https://github.com/graham-riches/rtos-plusplus#rtos-overview)
>- [Setting Up the Toolchain](https://github.com/graham-riches/rtos-plusplus#setting-up-the-toolchain)
>- [Building and Debugging](https://github.com/graham-riches/rtos-plusplus#building-and-debugging)
>- [Writing and Running Unit Tests]()


# HAL Overview
The main purpose of writing a custom HAL was to learn how to configure and use a microcontroller directly at the register level. The use of modern C++ (especially scoped enums) leads to a clean API when interacting with peripherals and makes it very easy to see what the HAL is doing. As an example, here is short snippet of code that configures the flash access control register:

```cpp
HAL::flash.set_access_control_register(HAL::FlashAccessControlRegister::prefetch_enable, 0x01);
HAL::flash.set_access_control_register(HAL::FlashAccessControlRegister::instruction_cache_enable, 0x01);
HAL::flash.set_access_control_register(HAL::FlashAccessControlRegister::data_cache_enable, 0x01);
HAL::flash.set_access_control_register(HAL::FlashAccessControlRegister::latency, 0x05);
```


## Currently Supported Peripherals

>- Power Management: currently supports register level control over the STM32 power control register
>- Reset and Control Clock: currently supports general purpose configuration of the RCC
>- Flash Access: currently supports general flash access control register configuration
>- GPIO: currently supports GPIO configuration and general purpose usage
>- EXTI: currently supports external interrupts on GPIO pins
>- USART: currently supports both polling and interrupt driven modes. There is currently no DMA support.
>- SPI: currently supports both polling and interrupt driven modes. There is currently no DMA support.
>- The Rest: currently WIP but please feel free to add support for anything :)

# RTOS Overview
This project contains a simple operating system with a cooperative scheduler that runs off of the Cortex SysTick interrupt. Context switches are handled via the PendSV interrupt and make use of how the ARM processor stacks the current register state of the executing context when jumping into the exception handler (interrupt). The remaining core registers are also pushed to the stack and then the next pending thread is loaded from a global pointer to the next thread the scheduler has selected to run. The stack pointer and register state from this state are then restored in the exception handler and the default stacked registers are popped when exiting from the interrupt. **NOTE** currently there is no support for storing floating point register context.


## OS Components
>- Scheduler: the OS uses a relatively simple scheduling algorithm to determine when to execute threads. The OS always maintains an internal IDLE task (similar to FreeRTOS) that runs when no other threads are active. Whenever a thread is ready to run, the scheduler will pick up it's pending state and schedule a context switch to the thread.
>- Threads: the OS currently supports threads via the threading API. Threads can be created with custom stack sizes as is typical in RTOS' applications. Currently the scheduler does not support thread priorities, but that is in the roadmap for the future! Threads can be put to sleep via some system calls and the scheduler will re-activate them once the alloted time has expired.
>- System Clock: the OS provides a millisecond accuracy system clock based on the SysTick interrupt that can be used to time application events
>- Semaphores: currently a work-in-progress (almost complete), but the OS will soon provide a semaphore mechanism for thread signalling
>- Mutex: also currently a work-in-progress, but the OS will soon provide a mechanism for mutual exclusion to prevent multi-threading shenanigans


## Sample Code
The following sample code shows off a bit of the syntax involved when using the OS:

```cpp
/*********************************** Consts ********************************************/
constexpr uint8_t thread_stack_size = 128;

/*********************************** Local Function Declarations ********************************************/
static void thread_one_task(void *arguments);
static void thread_two_task(void *arguments);

/*********************************** Local Variables ********************************************/
/* create the thread stacks */
static uint32_t thread_one_stack[thread_stack_size] = {0};
static uint32_t thread_two_stack[thread_stack_size] = {0};

/* create the threads */
static os::thread thread_one(thread_one_task, nullptr, 1, thread_one_stack, thread_stack_size);
static os::thread thread_two(thread_two_task, nullptr, 2, thread_two_stack, thread_stack_size);

/*********************************** Function Definitions ********************************************/
/**
  * \brief  Main application function
  * \retval int
  */
int main(void) {
    //!< register the threads
    os::scheduler::register_new_thread(&thread_one);
    os::scheduler::register_new_thread(&thread_two);

    //!< configure the project specific HAL drivers and bootup the chip, clocks etc.
    initialize_peripherals();    

    //!< jump into the RTOS kernel
    os::kernel::setup();
    os::kernel::enter();

    //!< NOTE: should never reach here
    return 0;
}

/**
 * \brief first task to blink two LEDs
 */
static void thread_one_task(void *arguments) {
    PARAMETER_NOT_USED(arguments);
    while ( true ) {        
        debug_port.send("a\n");
        green_led.toggle();
        blue_led.toggle();
        os::scheduler::sleep(100);
    }
}

/**
 * \brief second task to blink the other two LEDs
 */
static void thread_two_task(void *arguments) {
    PARAMETER_NOT_USED(arguments);
    while ( true ) {
        debug_port.send("b\n");
        red_led.toggle();
        orange_led.toggle();
        os::scheduler::sleep(100);
    }
}
```


# Setting Up the Toolchain
This project is currently built with the open source ARM embedded toolchain [available here](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm) and CMake, however it should be equally possible to build the code in an environment like System Workbench for STM32 or Rowley CrossWorks. The project currently has Tasks setup for use in VsCode to build, deploy, and debug the code on the microcontroller directly from VsCode.

## Installing Dependencies:
- Install the ARM embedded toolchain from the link above and add it to your PATH environment variable
- Install [CMake](https://cmake.org/download/) and add it to your PATH environment variable
- Install [MinGW](http://mingw-w64.org/doku.php) and add it to your PATH environment variable
- If using VsCode, install the following extensions: CMake, CMake Tools, C/C++, Tasks, Cortex-Debug

# Building and Debugging

## Building Steps
To build the code, run the following set of commands from the project top-level:

```
cmake -DCMAKE_TOOLCHAIN_FILE:arm-none-eabi-gcc.cmake -G"MinGW Makefiles" -S . -B build
cd build
mingw32-make
```

## Loading the Code
To be able to flash the code onto the microcontroller, the best bet is to use the Open On-Chip Debugger toolset (OpenOCD) [available here](https://sourceforge.net/projects/openocd/) (or windows ports [here](https://gnutoolchains.com/arm-eabi/openocd/)).


```
openocd -f ../stm32f407.cfg -c \"program bare_metal_os.elf verify reset exit\"
```

**NOTE** This functionality is already present with the "Load Build" task in VsCode (if you're using that environment).

## Debugging the Code
You can launch a debug session with OpenOCD using GDB directly, however it's a lot nicer (and easier IMO) to use the debug configuration already present in the VsCode project, which allows normal visual debugging with breakpoints (and the Cortex-Debug extension support peripheral views)!


# Building and Running Unit Tests
This project has a googletest project for unit testing some of the OS components. To build and run the unit tests (either linux or windows):
```
cd tests
cmake -S . -B build
cd build
make (or use msbuild if windows)
cd bin
./bare_metal_os_tests
```
