/****************************************************************
 * @file    cpu.c
 * @brief   CPU related HAL function implementations
 *
 * @copyright SPDX-License-Identifier: GPL-3.0-or-later
 ****************************************************************/

#include <hal.h>

static srv_cpu_t cpus[SYSRV_MAX_CPUS]; /**< Logical processor bookkeeping array */

uint32_t srv_hal_GetExecutingCPU(void)
{
    uint32_t cpu_num;

    __asm__ volatile("mv %0, tp"
                     : "=r"(cpu_num));

    return cpu_num;
}

srv_cpu_context_t* srv_hal_GetCurrentContextStruct(void)
{
    /* Get the CPU ID and use that as an index into our state array */
    const cpu_num = srv_hal_GetExecutingCPU();

    return &cpus[cpu_num].registers;
}
