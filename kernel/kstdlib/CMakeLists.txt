#
# Kernel Standard Library CMake file
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

cmake_minimum_required(VERSION 3.28)

project(srv_hal C)

# Set up the C Flags specifically for the HAL
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

set(KSTDLIB_FILES
    string.c
)

add_library(kstdlib STATIC ${KSTDLIB_FILES})
target_include_directories(kstdlib PUBLIC $ENV{SYSRV_ROOT}/kernel/kstdlib)
