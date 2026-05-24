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

srv_arch_init_result_t srv_arch_Init(srv_boot_info_t* boot_info)
{
    (void)kprintf("The System is being brought up...\n");

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

    return SRV_ARCH_INIT_SUCCESS;
}
