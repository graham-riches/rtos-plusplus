cmake_minimum_required(VERSION 3.15.3)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
set(CMAKE_SYSTEM_NAME Generic)

project(stm32_discovery)

enable_language(C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(APP_SOURCES 		      
      # HAL++ source files
      Src/HAL/hal_gpio.cpp
      Src/HAL/hal_rcc.cpp
      Src/HAL/hal_flash.cpp
      Src/HAL/hal_power.cpp
      Src/HAL/hal_usart.cpp
      Src/HAL/hal_interrupt.cpp
		
		# Application Source Files
		Src/Application/main.cpp
      Src/Application/Events/event.c      
      Src/Peripherals/GPIO/gpio.cpp
      Src/Peripherals/USART/usart.cpp
      Src/Peripherals/USART/debug_port.cpp
		Src/Peripherals/peripherals.cpp
		Src/Peripherals/stm32f4xx_it.c
		
		# Start Up File
        startup/startup_stm32f407xx.s)

set(BINARY ${PROJECT_NAME}.elf)

add_executable(${BINARY} ${APP_SOURCES})

target_compile_definitions(${BINARY} PRIVATE
		-DARM_MATH_CM4
		-DSTM32F40XX
		-DSTM32F407xx
        )

target_include_directories(${BINARY} PRIVATE
      Drivers/CMSIS/Device/ST/STM32F4xx/Include
		Drivers/CMSIS/Include
		Drivers/CMSIS/DSP_Lib/Include
		
      Src
      Src/HAL      
		Src/Application
		Src/Application/Accelerometer
		Src/Application/Audio
		Src/Application/Debug
		Src/Application/Events
		Src/Application/Utilities
		
		Src/Peripherals
		Src/Peripherals/GPIO		
		Src/Peripherals/USART
        )

target_compile_options(${BINARY} PRIVATE		
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard
        -fdata-sections
        -ffunction-sections
        -fno-exceptions
		  -O0
		  -g3
        -Wall
		  -Wextra
		  -fmessage-length=0        
        )

target_link_options(${BINARY} PRIVATE
        -T${CMAKE_SOURCE_DIR}/STM32F407VgTx_FLASH.ld
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard
        -specs=nosys.specs
        -lc
        -lm
        -lnosys
        -Wl,-Map=${PROJECT_NAME}.map,--cref
        -Wl,--gc-sections
        )

# Print executable size
add_custom_command(TARGET ${BINARY}
        POST_BUILD
        COMMAND arm-none-eabi-size ${BINARY})

# Create hex file
add_custom_command(TARGET ${BINARY}
        POST_BUILD
        COMMAND arm-none-eabi-objcopy -O ihex ${BINARY} ${PROJECT_NAME}.hex
        COMMAND arm-none-eabi-objcopy -O binary ${BINARY} ${PROJECT_NAME}.bin)