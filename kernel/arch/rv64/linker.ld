/**
 * Basic Kernel linker script
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2024-2025 Jesse Buhagiar
 */

OUTPUT_FORMAT(elf64-littleriscv)
ENTRY(_start)

__PHYSICAL_MEMORY_START      = 0x80000000;
__KERNEL_PHYSICAL_START      = 0x80200000;

/*
 * Kernel virtual region base: -2 GB in Sv39, matching the Linux RISC-V
 * address map (ffffffff80000000 - ffffffffffffffff).
 *
 * __KERNEL_VA_PA_OFFSET is the fixed delta between a kernel virtual address
 * and its physical address, derived from the single Sv39 gigapage that maps
 * 0xffffffff80000000 -> 0x80000000.  Stripping these upper bits from any
 * kernel VA gives its PA, i.e.  PA = VA - __KERNEL_VA_PA_OFFSET.
 */
__KERNEL_VIRTUAL_BASE        = 0xffffffff80000000;
__KERNEL_VA_PA_OFFSET        = __KERNEL_VIRTUAL_BASE - __PHYSICAL_MEMORY_START;

SECTIONS
{
    . = __KERNEL_PHYSICAL_START;

    .boot :
    {
        __boot_start = .;
        *(.init)
        *(.init.data)
        . = ALIGN(16);
        __boot_end = ABSOLUTE(.);
    }

    /*
     * Place all kernel sections immediately after the boot section, in both
     * physical memory (LMA = __boot_end) and virtual memory
     * (VMA = __boot_end + __KERNEL_VA_PA_OFFSET).
     *
     * This ensures the Sv39 gigapage (0xffffffff80000000 -> 0x80000000)
     * translates every kernel VMA exactly to its physical load address:
     *   LMA = VMA - __KERNEL_VA_PA_OFFSET
     */
    . = __boot_end + __KERNEL_VA_PA_OFFSET;
    PROVIDE(__kernel_virtual_start = .);

    .text : AT(ADDR(.text) - __KERNEL_VA_PA_OFFSET)
    {
        PROVIDE(__kernel_text_start = .);
        *(.text)
        . = ALIGN(4K);
        PROVIDE(__kernel_text_end = .);
    }

    .rodata : AT(ADDR(.rodata) - __KERNEL_VA_PA_OFFSET)
    {
        PROVIDE(__kernel_rodata_start = .);
        *(.rodata)
        . = ALIGN(4K);
        PROVIDE(__kernel_rodata_end = .);
    }

	.data ALIGN(8) : AT(ADDR(.data) - __KERNEL_VA_PA_OFFSET)
	{
		PROVIDE(__kernel_data_start = .);
		PROVIDE(__global_pointer$ = . + 0x800);
		*(.sdata .sdata.* .sdata2 .sdata2.*);
		*(.data)
        . = ALIGN(4K);
		PROVIDE(__kernel_data_end = .);
	}

    .bss ALIGN(8) (NOLOAD) : AT(ADDR(.bss) - __KERNEL_VA_PA_OFFSET)
    {
        PROVIDE(__kernel_bss_start = .);
        *(COMMON)
        *(.bss)
        . = ALIGN(4K);
        PROVIDE(__kernel_bss_end = .);
    }

    /* Configure the Kernel Stack pool */
    .kernel_stack (NOLOAD) :
    {
        __kernel_stack_bottom = .;
        . += 4K;
        __kernel_stack_top = .;
    }

    /* Allocate some space for the Eternal Kernel Heap */
    . = ALIGN(4K);
    __kalloc_eternal_start = .;
    . = . + 1M;
    __kalloc_eternal_end = .;

    . = ALIGN(4K);

    __kernel_end = .;
    PROVIDE(__kernel_virtual_end = .);
}
