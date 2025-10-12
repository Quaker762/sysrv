#
# System-RV Kernel CMakeLists.txt
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

cmake_minimum_required(VERSION 3.28)

project(srv_kern C ASM)

if ($ENV{SYSRV_ARCH} STREQUAL "rv64")
    set (SRV_KERN_NAME "kern64.elf")
    set (LINKER_SCRIPT_PATH $ENV{SYSRV_ROOT}/kernel/arch/rv64/linker.ld)
    set(KERNEL_SOURCE_FILES
        ${KERNEL_SOURCE_FILES}
        $ENV{SYSRV_ROOT}/kernel/boot/rv64/start.s
    )
endif()

# Set up the C Flags specifically for the HAL
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

# Add the HAL as an include path

set(KERNEL_SOURCE_FILES
    ${KERNEL_SOURCE_FILES}
    kmain.c
)

SET(LINKER_SCRIPT "${LINKER_SCRIPT_PATH}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -nostartfiles -T ${LINKER_SCRIPT} -Wl,-Map=${TARGET}.map -Wl,--gc-sections -lgcc -nostdlib -mcmodel=medany")

add_executable(${SRV_KERN_NAME} ${KERNEL_SOURCE_FILES})
target_link_libraries(${SRV_KERN_NAME} srv_hal)
target_link_libraries(${SRV_KERN_NAME} kstdlib)
