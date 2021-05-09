###################################################
# CMakeLists build script for bare metal os tests
#
# Author: Graham Riches
# Date:   November 21, 2020
# 
###################################################
cmake_minimum_required(VERSION 3.1...3.15)
project(bare-metal-os-tests)

# Set Language Standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# set the default build type to debug unless otherwise specified
set(DEFAULT_BUILD_TYPE "Debug")
if (NOT CMAKE_BUILD_TYPE)
   message("Setting default build type as none was specified")
   set(CMAKE_BUILD_TYPE ${DEFAULT_BUILD_TYPE})
endif()

# enable different build settings depending on the compiler used
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3 -Wall -Wextra")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2 -Wall -Wextra")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS_RELEASE "/MT")
    set(CMAKE_CXX_FLAGS_DEBUG "/MTd /Zi /Ob0 /Od /RTC1")
else()
    message(FATAL_ERROR "Unsupported compiler version")
endif()

# set the parent directory
get_filename_component(PARENT_DIR ${CMAKE_SOURCE_DIR} DIRECTORY)

# build gtest
add_subdirectory(googletest)

# set the binary and add custom sources as needed
set(BINARY bare-metal-os-tests)
set(SOURCES
    # add unit tests here
    #events_tests.cpp
    scheduler_tests.cpp
    threading_tests.cpp
    system_clock_tests.cpp
    ring_buffer_tests.cpp    

    # add each application file to test here
    ${PARENT_DIR}/source/OS/thread/thread_impl.cpp    
    )

add_executable(${BINARY} ${SOURCES})

# include the project source files
target_include_directories(${BINARY} PUBLIC
    ${PARENT_DIR}/source
    ${PARENT_DIR}/source/Utilities
    ${PARENT_DIR}/source/HW_Port
    ${PARENT_DIR}/source/HAL      
    ${PARENT_DIR}/source/Application
    ${PARENT_DIR}/source/Application/Accelerometer      
    ${PARENT_DIR}/source/Application/Audio
    ${PARENT_DIR}/source/Application/Debug
    ${PARENT_DIR}/source/Application/Utilities
    ${PARENT_DIR}/source/OS/
    ${PARENT_DIR}/source/OS/scheduler
    ${PARENT_DIR}/source/OS/sempahore
    ${PARENT_DIR}/source/OS/thread
    ${PARENT_DIR}/source/OS/mutex
    ${PARENT_DIR}/source/OS/system_clock
    ${PARENT_DIR}/source/Application/Peripherals			
    ${PARENT_DIR}/source/Application/Peripherals/USART
    ${PARENT_DIR}/source/Application/Peripherals/SPI
    ${CMAKE_SOURCE_DIR}/googletest/googletest/include
    )

target_compile_definitions( ${BINARY} PRIVATE
    -DMAX_THREAD_COUNT=8
)

add_test(NAME ${BINARY} COMMAND ${BINARY})

target_link_libraries(${BINARY} gtest gtest_main)