/**
 * Basic Kernel linker script
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2024-2025 Jesse Buhagiar
 */

OUTPUT_FORMAT(elf64-littleriscv)
ENTRY(_start)

PHDRS
{
    kernel_text PT_LOAD FLAGS(5);
    boot_text PT_LOAD FLAGS(5);
    kernel_data PT_LOAD FLAGS(6);
    boot_data PT_LOAD FLAGS(6);
}

__PHYSICAL_MEMORY_START      = 0x80000000;
__KERNEL_PHYSICAL_START      = 0x80200000;
__KERNEL_STACK_POOL_PHYSICAL = __PHYSICAL_MEMORY_START + 16M;

SECTIONS
{
    . = __KERNEL_PHYSICAL_START;

    .text : AT(ADDR(.text) + __PHYSICAL_MEMORY_START)
    {
        PROVIDE(__kernel_text_start = .);
        *(.text)
        . = ALIGN(4K);
        PROVIDE(__kernel_text_end = .);
    } : kernel_text

    .rodata : AT(ADDR(.rodata) + __PHYSICAL_MEMORY_START)
    {
        PROVIDE(__kernel_rodata_start = .);
        *(.rodata)
        . = ALIGN(4K);
        PROVIDE(__kernel_rodata_end = .);
    } : kernel_text

	.data ALIGN(8) : AT(ADDR(.data) + __PHYSICAL_MEMORY_START)
	{
		PROVIDE(__kernel_data_start = .);
		PROVIDE(__global_pointer$ = . + 0x800);
		*(.sdata .sdata.* .sdata2 .sdata2.*);
		*(.data)
        . = ALIGN(4K);
		PROVIDE(__kernel_data_end = .);
	} : kernel_data

    .bss ALIGN(8) (NOLOAD) : AT(ADDR(.bss) + __PHYSICAL_MEMORY_START)
    {
        PROVIDE(__kernel_bss_start = .);
        *(COMMON)
        *(.bss)
        . = ALIGN(4K);
        PROVIDE(__kernel_bss_end = .);
    }

    /* Configure the Kernel Stack pool */
    .kernel_stack (NOLOAD) : AT(__KERNEL_STACK_POOL_PHYSICAL)
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
