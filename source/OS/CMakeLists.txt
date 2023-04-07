# --------------------------------------------------------------------------------
# \brief This function configures the OS layer as a static library that can be linked
# to your specific application.
#
# \param port_directory Which device port to build the library for. Must be a valid
#        subdirectory under the Port directory
#
# \param max_thread_count Maximum number of threads to support
#
# \note This function creates a library called rtos++ that you must add into your
#       target_link_libraries
#
# \note This function will also set a variable called OS_LINKER_SCRIPT, which
#       is used in the main application to link the build to a specific device/startup
# --------------------------------------------------------------------------------
function(configure_rtos_libraries port_directory max_thread_count)
    set(OS_LIB_NAME rtos++)

    # Get device specific port files
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/source/OS/Port/${port_directory})

    # Build os files
    set(OS_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/source/OS/os.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/OS/scheduler.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/OS/thread.cpp

        # Add files from device port
        ${OS_FILES_TO_COMPILE}
    )

    # Add the library target
    add_library(${OS_LIB_NAME} STATIC ${OS_SOURCES})

    # Add include directories and publically export them so that they are automatically included
    # in any targets that link with this generated library
    target_include_directories(${OS_LIB_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/source/OS
        ${CMAKE_CURRENT_SOURCE_DIR}/source/OS/Port/${port_directory}
        ${CMAKE_CURRENT_SOURCE_DIR}/source/OS/Port/${port_directory}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/source/OS/Port/${port_directory}/src
    )    

    # Add specific compile definitions
    target_compile_definitions(${OS_LIB_NAME} PUBLIC 
        ${OS_PORT_COMPILE_DEFINITIONS}
        -DMAX_THREAD_COUNT=${max_thread_count}
    )
    
    # Set the linker script in the parent scope so that it's visible
    set(OS_LINKER_SCRIPT ${OS_PORT_LINKER_SCRIPT} PARENT_SCOPE)

    # Build options/compiler flags
    target_compile_options(${OS_LIB_NAME} PUBLIC
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
endfunction()
