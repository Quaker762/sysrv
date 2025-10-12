#
# Kernel Entrypoint
#
# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2025 Jesse Buhagiar
#

.type _start, @function
.global _start
.section .init
.extern __kernel_stack_top

#
# Kernel entry point from OpenSBI
#
# a0 - Hart ID
#
_start:
    # Load the initial stack pointer
    la t0, __kernel_stack_top
    mv sp, t0

    #
    # Only perform init if the Hart ID is 0 and park
    # any other Harts
    #
    lui t0, 0
    bne a0, t0, _boot_ParkHart

    jal kmain

_boot_ParkHart:
    j .
