#
# Kernel Standard Library CMake file
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

cmake_minimum_required(VERSION 3.28)

project(kstdlib C)

# Set up the C Flags specifically for the HAL
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

# Add the HAL as an include path
include_directories($ENV{SYSRV_ROOT}/hal)

set(KSTDLIB_FILES
    string.c
    stdio.c
)

add_library(kstdlib STATIC ${KSTDLIB_FILES})
target_include_directories(kstdlib PUBLIC $ENV{SYSRV_ROOT}/kernel/kstdlib)
