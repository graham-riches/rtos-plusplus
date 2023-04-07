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
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/exti
    ${CMAKE_CURRENT_SOURCE_DIR}/flash
    ${CMAKE_CURRENT_SOURCE_DIR}/gpio
    ${CMAKE_CURRENT_SOURCE_DIR}/nvic
    ${CMAKE_CURRENT_SOURCE_DIR}/power_management
    ${CMAKE_CURRENT_SOURCE_DIR}/rcc
    ${CMAKE_CURRENT_SOURCE_DIR}/spi
    ${CMAKE_CURRENT_SOURCE_DIR}/usart
    
    # TODO: this is a hack as the HAL is not currently very well setup for different targets
    ${CMAKE_SOURCE_DIR}/source/OS/Port/stm32f407/
    ${CMAKE_SOURCE_DIR}/source/OS/Port/stm32f407/include
    
)

# Add compile time definitions
target_compile_definitions(${HAL_LIBRARY} PRIVATE    
    -DARM_MATH_CM4
    -DSTM32F40XX
    -DSTM32F407xx
    -D__FPU_PRESENT 
    -D__FPU_USED   
)

# Compile options for HAL library
target_compile_options(${HAL_LIBRARY} PRIVATE    
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
