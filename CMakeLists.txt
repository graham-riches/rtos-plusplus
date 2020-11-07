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
      source/HAL/hal_gpio.cpp
      source/HAL/hal_rcc.cpp
      source/HAL/hal_flash.cpp
      source/HAL/hal_power.cpp
      source/HAL/hal_usart.cpp
      source/HAL/hal_interrupt.cpp
		
		# Application Source Files
      source/Application/main.cpp      
      source/Application/Events/event.cpp  
      source/Application/Peripherals/GPIO/gpio.cpp
      source/Application/Peripherals/USART/usart.cpp
      source/Application/Peripherals/USART/debug_port.cpp
		source/Application/Peripherals/peripherals.cpp
		
      
      # OS files
      source/OS/scheduler.cpp
      source/OS/threading.cpp
      source/OS/timer.cpp
      source/OS/interrupts.cpp

		# Start Up File
      startup/startup_stm32f407xx.s
   )

set(BINARY ${PROJECT_NAME}.elf)

add_executable(${BINARY} ${APP_SOURCES})

target_compile_definitions(${BINARY} PRIVATE
		-DARM_MATH_CM4
		-DSTM32F40XX
      -DSTM32F407xx
      -D__FPU_PRESENT
        )

target_include_directories(${BINARY} PRIVATE
      Drivers/CMSIS/Device/ST/STM32F4xx/Include
		Drivers/CMSIS/Include
		Drivers/CMSIS/DSP_Lib/Include
		
      source
      source/HAL      
		source/Application
      source/Application/Accelerometer      
		source/Application/Audio
		source/Application/Debug
		source/Application/Events
		source/Application/Utilities
      
      source/OS/

		source/Application/Peripherals
		source/Application/Peripherals/GPIO		
      source/Application/Peripherals/USART
      )

target_compile_options(${BINARY} PRIVATE		
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard
        -fno-data-sections
        -fno-function-sections
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
        COMMAND arm-none-eabi-objcopy -O binary ${BINARY} ${PROJECT_NAME}.bin
        )

# Create disassembly
add_custom_command( TARGET ${BINARY}
         POST_BUILD
         COMMAND arm-none-eabi-objdump -d ${BINARY} > ${PROJECT_NAME}_dasm.txt)

# flash the build
add_custom_command( TARGET ${BINARY}
         POST_BUILD
         COMMAND openocd -f ../stm32f407.cfg -c "program stm32_discovery.elf verify reset exit")
		 
# build and run the unit tests
#add_subdirectory(tests)
