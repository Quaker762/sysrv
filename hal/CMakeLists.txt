#
# Top-level CMake file for SRV HAL files
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

if ($ENV{SYSRV_ARCH} STREQUAL "rv64")
    add_subdirectory(rv64)
else()
    message(FATAL_ERROR "Cannot build HAL with SYSRV_ARCH envvar unset!")
endif()
