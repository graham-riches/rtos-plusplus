<div id="top"></div>

<!-- PROJECT LOGO -->
<br />
<div align="center">
  <h1 align="center">RTOS++</h1>

  <p align="center">
    A modern C++ (C++20) Real-Time Operating System (RTOS) and Hardware Abstraction Layer (HAL) for Cortex-M4 Microcontrollers
    <br />
    <a href="https://github.com/graham-riches/rtos-plusplus/issues">Report Bug</a>
    ·
    <a href="https://github.com/graham-riches/rtos-plusplus/issues">Request Feature</a>
  </p>
</div>


<!-- TABLE OF CONTENTS -->
<details open>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href=#rtos-overview>RTOS Overview</a></li>
        <li><a href=#hal-overview>HAL Overview</a></li>
      </ul>      
    </li>    
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#setting-up-the-toolchain">Setting Up the Toolchain</a></li>
        <li><a href="#building-and-loading-the-code">Building and Loading the Code</a></li>
        <li><a href="#debugging">Debugging</a></li>
        <li><a href="#building-and-running-unit-tests">Building and Running Unit Tests</a></li>
      </ul>
    </li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
  </ol>
</details>


# About The Project
This project originally started as a personal passion project aimed to dispell the commonly held belief in embedded systems programming (particularly in the microcontroller world) that C is the only suitable language to be used. Modern C++ provides so many great features that make microcontroller programming easier, and I wanted to create an environment for embedded programming that mostly looks and feels just like writing standard C++ for any other operating system (e.g. Windows, Linux, etc.). With that in mind, most of the operating system APIs are designed to closely mimic those offered by the C++ standard library to make transitioning into embedded programming less intimidating!
<p align="right">(<a href="#top">back to top</a>)</p>



## RTOS Overview
This project contains a simple real-time operating system with a cooperative scheduler that runs off of the Cortex SysTick interrupt. Context switches are handled via the PendSV interrupt and make use of how the ARM processor stacks the current register state of the executing context when jumping into the exception handler. The remaining core registers are also pushed to the stack and then the next pending thread is loaded from a global pointer to the next thread the scheduler has selected to run. The stack pointer and register state from this state are then restored in the exception handler and the default stacked registers are popped when exiting from the interrupt. **NOTE** currently there is no support for storing floating point register context in threads!
<p align="right">(<a href="#top">back to top</a>)</p>


### OS Components
>- Scheduler: The OS uses a relatively simple scheduling algorithm to determine when to execute threads. The OS always maintains an internal IDLE task (similar to FreeRTOS) that runs when no other threads are active. Whenever a thread is ready to run, the scheduler will pick it up and schedule a context switch.
>- Threads: The OS currently supports threads via the threading API. Threads can be created with custom stack sizes as is typical in RTOS applications. Currently the scheduler does not support thread priorities, but that is in the roadmap for the future! Threads can be suspended or put to sleep for a fixed period of time using the threading API.
>- System Clock: The OS provides a millisecond accuracy system clock based on the SysTick interrupt that can be used to time application events, or sleeps
>- Semaphores: The OS provides semaphores for synchronization via the `os::counting_semaphore` and `os::binary_semaphore` classes.
<p align="right">(<a href="#top">back to top</a>)</p>

### Sample Code
The following sample code shows off a bit of the syntax involved when using the OS:

```cpp
constexpr uint8_t thread_stack_size = 128;

static void thread_one_task();
static void thread_two_task();

static uint32_t thread_one_stack[thread_stack_size] = {0};
static uint32_t thread_two_stack[thread_stack_size] = {0};
static os::binary_semaphore sem(0);

int main(void) {
    // Create two threads
    os::thread thread_one(thread_one_task, 1, thread_one_stack, thread_stack_size);
    os::thread thread_two(thread_two_task, 2, thread_two_stack, thread_stack_size);

    // Configure peripherals
    initialize_peripherals();

    // Jump into the RTOS kernel
    os::kernel::setup();
    os::kernel::enter();

    //!< NOTE: should never reach here!
    return 0;
}

// First thread will blink to LEDs and signal the second task to wakeup via the shared semaphore
static void thread_one_task() {    
    while ( true ) {        
        green_led.toggle();
        blue_led.toggle();        
        os::scheduler::sleep(1000);
        sem.release();
    }
}

// Second thread blinks the other two LEDs and logs a message
static void thread_two_task() {    
    while ( true ) {
        sem.acquire();
        debug_port.log_message("In second thread\r\n");
        red_led.toggle();
        orange_led.toggle();        
    }
}
```
<p align="right">(<a href="#top">back to top</a>)</p>

## HAL Overview

The main purpose of writing a custom HAL is to enable more readable code that better communicates what is happening at the register level with peripheral. For example, common register level code written in C can often be very obfuscated and hard to understand. Using C++ features like scoped enums, namespaces, and templates, we can create APIs that are not only high-performance, but also clearly communicate the intent of the programmer while allowing for better abstraction and encapsulation! As an example, the code snippet below shows some common MCU setup code that configures the flash access control register.

```cpp
flash::set_access_control_register_bit(flash::access_control_register::prefetch_enable, 0x01);
flash::set_access_control_register_bit(flash::access_control_register::instruction_cache_enable, 0x01);
flash::set_access_control_register_bit(flash::access_control_register::data_cache_enable, 0x01);
flash::set_access_control_register_bit(flash::access_control_register::latency, 0x05);
```
<p align="right">(<a href="#top">back to top</a>)</p>

### Currently Supported Peripherals
The HAL layer is currently quite bare-bones and support for new peripherals is only added as needed. The following list shows which HAL features are implemented at this point in time:

>- Power Management: Currently supports register level control over the STM32 power control register
>- Reset and Control Clock: Currently supports general purpose configuration of the RCC
>- Flash Access: Currently supports general flash access control register configuration
>- GPIO: Currently supports GPIO configuration and general purpose usage
>- EXTI: Currently supports external interrupts on GPIO pins
>- USART: Currently supports basic configuration of USART peripherals. For interrupt/DMA driven support, the user has to implement their own ISRs etc.
>- SPI: Same as USART
>- The Rest: Please implement and submit a PR!
<p align="right">(<a href="#top">back to top</a>)</p>

# Getting Started


## Setting Up the Toolchain
This project is currently built with the open source ARM embedded toolchain  and CMake, however it should be equally possible to build the code in an environment like System Workbench for STM32 if desired. The project currently has Tasks setup for use in VsCode to build, deploy, and debug the code on the microcontroller directly from VsCode.
<p align="right">(<a href="#top">back to top</a>)</p>

### Installing Dependencies:

#### Windows
- Install the ARM embedded toolchain from the link above and add it to your PATH environment variable [available here](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm)
- Install [CMake](https://cmake.org/download/) and add it to your PATH environment variable
- Install [MinGW](http://mingw-w64.org/doku.php) and add it to your PATH environment variable
- If using VsCode, install the following extensions: CMake, CMake Tools, C/C++, Tasks, Cortex-Debug
<p align="right">(<a href="#top">back to top</a>)</p>

#### Linux
- Install the arm embedded toolchain (`arm-none-eabi`) (either via `apt-get`, `pacman` etc.)
- Install the `arm-none-eabi-gdb` package to enable debugging
- Install the `stlink-tools` package to enable use of ST-Link debugger/flasher
- Install `open-ocd` package to use the open source debugger: [Available Here](https://openocd.org/pages/getting-openocd.html)
<p align="right">(<a href="#top">back to top</a>)</p>

## Building and Loading the Code

### Windows Building Steps
To build the code on Windows, run the following set of commands from the project top-level. Note: This might be out of date!

``` bash
cmake -DCMAKE_TOOLCHAIN_FILE:arm-none-eabi-gcc.cmake -G"MinGW Makefiles" -S . -B build
cd build
mingw32-make
```

To load the code:
``` bash
openocd -f ../stm32f407.cfg -c \"program bare_metal_os.elf verify reset exit\"
```

**NOTE** This functionality is already present with the "Load Build" task in VsCode (if you're using that environment).
<p align="right">(<a href="#top">back to top</a>)</p>

### Building on Linux

- Setup the build system using the top-level make file, which will call CMake with the right toolchain: `make build`
- Flash the build via: `make load`

```bash
make build
make load
```
<p align="right">(<a href="#top">back to top</a>)</p>


## Debugging 
You can launch a debug session with OpenOCD using GDB directly, however it's a lot nicer (and easier IMO) to use the debug configuration already present in the VsCode project, which allows normal visual debugging with breakpoints (and the Cortex-Debug extension supports peripheral views). 

- Cortex-Debug Extension - [Link Here](https://marketplace.visualstudio.com/items?itemName=marus25.cortex-debug)

<p align="right">(<a href="#top">back to top</a>)</p>


## Building and Running Unit Tests
This project has a googletest project for unit testing some of the OS components. To build and run the unit tests (either linux or windows):

```bash
cd tests
cmake -S . -B build
cd build
make
cd bin
./bare_metal_os_tests
```

## Contributing

Any contributions are welcome! Please feel free to submit and new features or fixes through a pull request.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- LICENSE -->
## License

Distributed under the GPL-3.0 License. See `LICENSE` for more information.

<p align="right">(<a href="#top">back to top</a>)</p>
