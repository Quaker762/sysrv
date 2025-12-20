/****************************************************************
 * @file    kmain.c
 * @brief   Kernel main function
 *
 * @copyright SPDX-License-Identifier: GPL-3.0-or-later
 ****************************************************************/

#include <stdio.h>

#include <mm/phys/kpalloc.h>
#include <drivers/fdt/fdt.h>
#include <panic.h>

int kmain(void)
{
    kprintf("Hello, World!\n");

    for (;;)
    {
    }

    return 0;
}
