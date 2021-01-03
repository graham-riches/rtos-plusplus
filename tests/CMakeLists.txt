###################################################
# CMakeLists build script for bare metal os tests
#
# Author: Graham Riches
# Date:   November 21, 2020
# 
# 
###################################################
cmake_minimum_required(VERSION 3.1...3.15)
project(bare-metal-os-tests)

set(CMAKE_CXX_FLAGS_RELEASE "/MT")
set(CMAKE_CXX_FLAGS_DEBUG "/MTd /Zi /Ob0 /Od /RTC1")


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
	main.cpp

	# add each application file to test here
    #${PARENT_DIR}/source/OS/events.cpp
    ${PARENT_DIR}/source/OS/scheduler.cpp
    ${PARENT_DIR}/source/OS/threading.cpp
    ${PARENT_DIR}/source/OS/system_clock.cpp
	)

add_executable(${BINARY} ${SOURCES})


# include the project source files
target_include_directories(${BINARY} PRIVATE
	${PARENT_DIR}/Drivers/CMSIS/Device/ST/STM32F4xx/Include
	${PARENT_DIR}/Drivers/CMSIS/Include
	${PARENT_DIR}/Drivers/CMSIS/DSP_Lib/Include
	${PARENT_DIR}/source
	${PARENT_DIR}/source/HAL      
	${PARENT_DIR}/source/Application
	${PARENT_DIR}/source/Application/Accelerometer      
	${PARENT_DIR}/source/Application/Audio
	${PARENT_DIR}/source/Application/Debug
	${PARENT_DIR}/source/Application/Utilities
	${PARENT_DIR}/source/OS/
	${PARENT_DIR}/source/Application/Peripherals			
	${PARENT_DIR}/source/Application/Peripherals/USART
	${PARENT_DIR}/source/Application/Peripherals/SPI
	)


add_test(NAME ${BINARY} COMMAND ${BINARY})

target_link_libraries(${BINARY} gtest gtest_main)