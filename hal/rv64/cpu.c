/****************************************************************
 * @file    cpu.c
 * @brief   CPU related HAL function implementations
 *
 * @copyright SPDX-License-Identifier: GPL-3.0-or-later
 ****************************************************************/

#include <hal.h>

uint32_t srv_hal_GetExecutingCPU(void)
{
    uint32_t cpu_num;

    __asm__ volatile("mv %0, tp"
                     : "=r"(cpu_num));

    return cpu_num;
}
