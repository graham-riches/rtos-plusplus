# Add a new library for the HAL
set(HAL_LIBRARY hal_library)

# Set the device name which is used to find device specific HAL/OS port
set(DEVICE_PORT_NAME stm32f4)

# Set files to build as part of the HAL
set(HAL_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/exti/hal_exti.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/flash/hal_flash.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/power_management/hal_power.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rcc/hal_rcc.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/spi/spi_base.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/spi/spi_polling.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/usart/usart_base.cpp    
)

# Create a new library target
add_library(${HAL_LIBRARY} ${HAL_SOURCES})

# Include directories of HAL
target_include_directories(${HAL_LIBRARY} PUBLIC
    ${CMAKE_SOURCE_DIR}/source/HW_Port
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/exti
    ${CMAKE_CURRENT_SOURCE_DIR}/flash
    ${CMAKE_CURRENT_SOURCE_DIR}/gpio
    ${CMAKE_CURRENT_SOURCE_DIR}/nvic
    ${CMAKE_CURRENT_SOURCE_DIR}/power_management
    ${CMAKE_CURRENT_SOURCE_DIR}/rcc
    ${CMAKE_CURRENT_SOURCE_DIR}/spi
    ${CMAKE_CURRENT_SOURCE_DIR}/usart        
    
)

# Add compile time definitions
target_compile_definitions(${HAL_LIBRARY} PRIVATE    
    -DARM_MATH_CM4
    -DSTM32F40XX
    -DSTM32F407xx
    -D__FPU_PRESENT    
)

# Compile options for HAL library
target_compile_options(${HAL_LIBRARY} PRIVATE    
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -fno-data-sections
    -fno-function-sections
    -fno-exceptions
    -O2
    -g0
    -Wall
    -Wextra
    -fmessage-length=0        
)
