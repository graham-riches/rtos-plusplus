#-------------------------------------------------------------------------------
# Project Setup
cmake_minimum_required(VERSION 3.15.3)
project(bare_metal_os)
set(CMAKE_VERBOSE_MAKEFILE off)


#-------------------------------------------------------------------------------
# Language Standards
enable_language(C CXX ASM)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


#-------------------------------------------------------------------------------
# Add submodules/subdirectories
add_subdirectory(${CMAKE_SOURCE_DIR}/source/HAL)
add_subdirectory(${CMAKE_SOURCE_DIR}/source/OS)


#-------------------------------------------------------------------------------
# Pre-Processor Compile Definitions
set(COMPILE_DEFINITIONS
    -DARM_MATH_CM4
    -DSTM32F40XX
    -DSTM32F407xx
    -D__FPU_PRESENT
    -D__FPU_USED
)

#-------------------------------------------------------------------------------
# Configure RTOS layer
set(MAX_THREAD_COUNT 8)
configure_rtos_libraries(stm32f407 ${MAX_THREAD_COUNT})


#-------------------------------------------------------------------------------
# Set files to compile for the application
set(SOURCES 		      
    # Application Source Files
    source/Application/system_startup.cpp
    source/Application/main.cpp
    source/Application/Peripherals/debug_port.cpp
    source/Application/Peripherals/peripherals.cpp
)


#-------------------------------------------------------------------------------
# Setup the Build Target
set(BINARY ${PROJECT_NAME}.elf)
add_executable(${BINARY} ${SOURCES})

target_compile_definitions(${BINARY} PRIVATE
    ${COMPILE_DEFINITIONS}
)


#-------------------------------------------------------------------------------
# Include Directories
target_include_directories(${BINARY} PRIVATE
    source
    source/Application    
    source/Application/Debug
    source/Application/Utilities
    source/Application/Peripherals			
    source/Application/Peripherals/USART
    source/Application/Peripherals/SPI
    source/Utilities
)


#-------------------------------------------------------------------------------
# Link Libraries
target_link_libraries(${BINARY} PUBLIC rtos++ hal_library)


#-------------------------------------------------------------------------------
# Compile Options/Flags and Linker Options
target_compile_options(${BINARY} PRIVATE    
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -fdata-sections
    -ffunction-sections
    -fno-exceptions
    -fno-rtti
    $<$<CONFIG:DEBUG>:-O0>
    $<$<CONFIG:RELEASE>:-O2>
    $<$<CONFIG:DEBUG>:-g3>
    $<$<CONFIG:RELEASE>:-g0> 
    -Wall
    -Wextra
    -fmessage-length=0        
)

# Set the linker options
target_link_options(${BINARY} PRIVATE
    -T${OS_LINKER_SCRIPT}
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -specs=nosys.specs
    -specs=nano.specs
    -lc
    -lm
    -lnosys
    -Wl,-Map=${PROJECT_NAME}.map,--cref
    -Wl,--gc-sections
    --no-exceptions
    -Wl,--undefined,_printf_float
    -Wl,--undefined,_scanf_float  
)


#-------------------------------------------------------------------------------
# Post-Build Steps

# Print executable size
add_custom_command(TARGET ${BINARY}
    POST_BUILD
    COMMAND arm-none-eabi-size ${BINARY}
)

# Create hex file
add_custom_command(TARGET ${BINARY}
    POST_BUILD
    COMMAND arm-none-eabi-objcopy -O ihex ${BINARY} ${PROJECT_NAME}.hex
    COMMAND arm-none-eabi-objcopy -O binary ${BINARY} ${PROJECT_NAME}.bin
)

# Create disassembly
add_custom_command( TARGET ${BINARY}
    POST_BUILD
    COMMAND arm-none-eabi-objdump -d ${BINARY} > ${PROJECT_NAME}_dasm.txt
)
