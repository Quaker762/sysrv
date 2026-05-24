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
__KERNEL_VIRTUAL_START       = 0xffffffff80200000;

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

    . = __KERNEL_VIRTUAL_START;

    HIDDEN(__KERNEL_VA_CODE_OFFSET = __KERNEL_VIRTUAL_START - __boot_end);
    .text : AT(ADDR(.text) - __KERNEL_VA_CODE_OFFSET)
    {

        PROVIDE(__kernel_text_start = .);
        *(.text)
        . = ALIGN(4K);
        PROVIDE(__kernel_text_end = .);
    }

    .rodata : AT(ADDR(.rodata) - __KERNEL_VA_CODE_OFFSET)
    {
        PROVIDE(__kernel_rodata_start = .);
        *(.rodata)
        . = ALIGN(4K);
        PROVIDE(__kernel_rodata_end = .);
    }

	.data ALIGN(8) : AT(ADDR(.data) - __KERNEL_VA_CODE_OFFSET)
	{
		PROVIDE(__kernel_data_start = .);
		PROVIDE(__global_pointer$ = . + 0x800);
		*(.sdata .sdata.* .sdata2 .sdata2.*);
		*(.data)
        . = ALIGN(4K);
		PROVIDE(__kernel_data_end = .);
	}

    .bss ALIGN(8) (NOLOAD) : AT(ADDR(.bss) - __KERNEL_VA_CODE_OFFSET)
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
}
