# Globs to get all files within the current port context
file(GLOB_RECURSE OS_PORT_SOURCES LIST_DIRECTORIES false *.h *.c *.cpp *.hpp)
set(OS_FILES_TO_COMPILE ${OS_PORT_SOURCES} PARENT_SCOPE)

# Set linker script to be used for this port
set(OS_PORT_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/LinkerScript.ld PARENT_SCOPE)

# Set port compile definitions
set(OS_PORT_COMPILE_DEFINITIONS
    -DARM_MATH_CM4
    -DSTM32F40XX
    -DSTM32F407xx
    -D__FPU_PRESENT
    -D__FPU_USED

    PARENT_SCOPE
)
