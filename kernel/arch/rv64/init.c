/****************************************************************
 * @file    init.c
 * @brief   Architecture Specific initialization routine
 *
 * @copyright SPDX-License-Identifier: GPL-3.0-or-later
 ****************************************************************/

#include <arch/arch.h>

#include <drivers/fdt/fdt.h>

srv_arch_init_result_t srv_arch_Init(srv_boot_info_t* boot_info)
{
    (void)boot_info;
    return SRV_ARCH_INIT_SUCCESS;
}
