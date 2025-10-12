#
# RV64 HAL CMakeLists file
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

cmake_minimum_required(VERSION 3.28)

project(srv_hal C)

# Set up the C Flags specifically for the HAL
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

set(HAL_SOURCES
    irq.c
)

add_library(srv_hal STATIC ${HAL_SOURCES})
target_include_directories(srv_hal PUBLIC $ENV{SYSRV_ROOT}/hal)
