#
# Kernel Entrypoint
#
# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2025 Jesse Buhagiar
#

# Define a Sv39 page table (512 entries, 4KB)
.macro DEFINE_PT name
    .align 12                   # 4K alignment
    .global \name
    \name:
    .fill 512, 8, 0            # 512 x 64-bit entries, all zero
.endm

.section .init.data
.global boot_info
boot_info:
    .quad 0 # rv64_boot_info_t::kernel_physical_address
    .quad 0 # rv64_boot_info_t::kernel_load_offset
    .quad 0 # rv64_boot_info_t::fdt_ptr
    .quad 0 # rv64_boot_info_t::physical_memory_base


DEFINE_PT __boot_root_page_table  # Root page table

.type _start, @function
.global _start
.section .init
.extern __kernel_stack_top
.extern __boot_start
.extern srv_arch_Init


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
    mv tp, a0 # Store the number of this CPU in the $tp register (which is unused by the Kernel)
    bne a0, t0, _boot_ParkHart
    mv t6, a1 # Move the FDT pointer into t6 so we can use it later

    #
    # Configure the boot page tables
    #

    # Identity map the physical region so execution can continue after MMU is enabled
    la a0, __boot_root_page_table
    la a1, __boot_start
    mv a2, a1                          # virtual == physical
    jal ra, __boot_map_virt_to_phys

    # Map the kernel to its high virtual address
    la a0, __boot_root_page_table
    la a1, __boot_start
    li a2, 0xFFFFFFFF00000000
    add a2, a2, a1
    jal ra, __boot_map_virt_to_phys

    srli a0, a0, 12
    li t1, 8
    slli t1, t1, 60                # Shift mode to bits 63:60
    or   a0, a0, t1                # Combine PPN and MODE

    csrw satp, a0
    sfence.vma

    # Set up the boot param structure

    # Construct the boot info structure
    la a0, boot_info
    la t0, __KERNEL_PHYSICAL_START
    la t1, __PHYSICAL_MEMORY_START
    sub t1, t0, t1
    sd t0, 0(a0)
    sd t1, 8(a0)
    sd t6, 16(a0)
    sd t1, 24(a0)

    #
    # Okay, now we're running with Paging enabled, so continue
    # on with the boot
    #

    # Load the initial stack pointer
    la t0, __kernel_stack_top
    mv sp, t0

    # Save a0 on the stack so we can get it back later
    addi sp, sp, -8
    sd a0, 0(sp)

    ld a0, 0(sp)

    # Get the address of the Kernel init function and jump to it
    la t0, srv_arch_Init
    jalr t0

    # If the Kernel returns then we need to stop here so we don't crash
    j .

#
# Map a virtual address to a physical address (Sv39 gigapage)
#
# a0 - Pointer to the root page table
# a1 - Physical Address (1 GiB-aligned base used for PPN[2])
# a2 - Virtual Address  (VPN[2] selects the root page table entry)
#
__boot_map_virt_to_phys:
    # Derive VPN[2] from the virtual address (a2) to select the root table entry
    srli t0, a2, 30
    andi t0, t0, 0x1FF

    srli t1, a1, 30         # t1 = PPN[2]
    slli t1, t1, 28         # t1 = PPN[2] << 28
    li   t2, 0xF            # I'm really lazy tonight; this is the FLAGS field of XWRV
    or   t1, t1, t2

    # Write PTE to root_pt[VPN[2]]
    slli t0, t0, 3          # byte offset = VPN[2] * 8
    add  t0, a0, t0
    sd   t1, 0(t0)

    jr ra

_boot_ParkHart:
    j .
