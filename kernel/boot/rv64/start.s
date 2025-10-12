#
# Kernel Entrypoint
#
# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2025 Jesse Buhagiar
#

.section .init.data
boot_info:
    .quad 0 # rv64_boot_info_t::kernel_physical_address
    .quad 0 # rv64_boot_info_t::kernel_load_offset
    .quad 0 # rv64_boot_info_t::fdt_ptr


.type _start, @function
.global _start
.section .init
.extern arch_Init
.extern __init_stack_top
.extern boot_SetupInitialPageTables

#
# Kernel entry point from OpenSBI
#
# a0 - Hart ID
#
_start:
    #
    # Only perform init if the Hart ID is 0 and park
    # any other Harts
    #
    lui t0, 0
    bne a0, t0, _boot_ParkHart

    jal kmain

_boot_ParkHart:
    j .
