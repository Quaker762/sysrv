#
# Kernel Entrypoint
#
# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2025 Jesse Buhagiar
#


.section .init.data
.global boot_info
boot_info:
    .quad 0 # rv64_boot_info_t::kernel_physical_address
    .quad 0 # rv64_boot_info_t::kernel_load_offset
    .quad 0 # rv64_boot_info_t::fdt_ptr

.type _start, @function
.global _start
.section .init
.extern __kernel_stack_top
.extern srv_arch_Init

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
    mv tp, a0 # Store the number of this CPU in the $tp register (which is unused by the Kernel)
    bne a0, t0, _boot_ParkHart

    # Set up the boot param structure

    # Construct the boot info structure
    la a0, boot_info
    la t0, __KERNEL_PHYSICAL_START
    la t1, __PHYSICAL_MEMORY_START
    sub t1, t0, t1
    sd t0, 0(a0)
    sd t1, 8(a0)
    sd a1, 16(a0)

    # Save a0 on the stack so we can get it back later
    addi sp, sp, -8
    sd a0, 0(sp)

    ld a0, 0(sp)

    # Get the address of the Kernel init function and jump to it
    la t0, srv_arch_Init
    jalr t0

_boot_ParkHart:
    j .
