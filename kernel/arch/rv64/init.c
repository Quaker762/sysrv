/****************************************************************
 * @file    init.c
 * @brief   Architecture Specific initialization routine
 *
 * @copyright SPDX-License-Identifier: GPL-3.0-or-later
 ****************************************************************/

#include <arch/arch.h>

#include <drivers/fdt/fdt.h>
#include <kstdlib/stdio.h>
#include <mm/phys/kpalloc.h>
#include <panic.h>

extern int kmain(void);

extern trap_handler_t trap_handler;

srv_arch_init_result_t srv_arch_Init(srv_boot_info_t* boot_info)
{
    (void)kprintf("The System is being brought up...\n");

    /* Install the trap handler */
    srv_hal_InstallTrapHandler(&trap_handler);

    /* Initialize the device tree */
    const bool fdt_init_ok = srv_fdt_Init(boot_info->fdt_ptr);
    if (fdt_init_ok == false)
    {
        return SRC_ARCH_INIT_DEVICE_TREE_INVALID;
    }

    /* Get the size of the RAM installed in the system */
    const size_t memory_size = srv_fdt_GetMemorySize();
    (void)kprintf("%lx bytes of RAM detected @ 0x%lx\n", memory_size, boot_info->physical_memory_base);

    /* Initialize the PMM */
    srv_kpalloc_InitPageAllocator(boot_info->physical_memory_base, memory_size);

    /*
     * We need to mark a few regions as unusable, namely:
     *
     * 1. The FDT
     * 2. The eternal heap (part of the Kernel image in the linker map)
     * 3. The Kernel image itself
     */
    const size_t fdt_size = ((srv_fdt_GetFDTSize() + SRV_PAGE_SIZE - 1) / SRV_PAGE_SIZE) * SRV_PAGE_SIZE; /* Make sure this is rounded to a page size */
    srv_kpalloc_MarkRegionUnusable((srv_physical_address_t)boot_info->fdt_ptr, fdt_size);

    extern srv_physical_address_t __kernel_virtual_start;
    extern srv_physical_address_t __kernel_virtual_end;

    const size_t                 kernel_region_length = ((srv_physical_address_t)&__kernel_virtual_end - (srv_physical_address_t)&__kernel_virtual_start);
    const srv_physical_address_t kernel_phys_start    = ((srv_physical_address_t)&__kernel_virtual_start - SRV_PAGING_KERNEL_REGION_BASE);
    srv_kpalloc_MarkRegionUnusable(kernel_phys_start, kernel_region_length);

    uint32_t*               blah = (uint32_t*)NULL;
    const volatile uint32_t test = *blah;
    (void)test;

    /* Branch to the Kernel's main function */
    kmain();

    return SRV_ARCH_INIT_SUCCESS;
}
