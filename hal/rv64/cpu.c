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
    const uint32_t cpu_num = srv_hal_GetExecutingCPU();

    return &cpus[cpu_num].registers;
}

void hal_SaveContextToProcControlBlock(const srv_cpu_context_t* context)
{
    const uint32_t cpu_num = srv_hal_GetExecutingCPU();

    srv_cpu_context_t* our_context = &cpus[cpu_num].registers;

    /* Copy each register from the provided context into our CPU control block. */
    our_context->zero = context->zero;
    our_context->ra   = context->ra;
    our_context->sp   = context->sp;
    our_context->gp   = context->gp;
    our_context->tp   = context->tp;
    our_context->t0   = context->t0;
    our_context->t1   = context->t1;
    our_context->t2   = context->t2;
    our_context->s0   = context->s0;
    our_context->s1   = context->s1;
    our_context->a0   = context->a0;
    our_context->a1   = context->a1;
    our_context->a2   = context->a2;
    our_context->a3   = context->a3;
    our_context->a4   = context->a4;
    our_context->a5   = context->a5;
    our_context->a6   = context->a6;
    our_context->a7   = context->a7;
    our_context->s2   = context->s2;
    our_context->s3   = context->s3;
    our_context->s4   = context->s4;
    our_context->s5   = context->s5;
    our_context->s6   = context->s6;
    our_context->s7   = context->s7;
    our_context->s8   = context->s8;
    our_context->s9   = context->s9;
    our_context->s10  = context->s10;
    our_context->s11  = context->s11;
    our_context->t3   = context->t3;
    our_context->t4   = context->t4;
    our_context->t5   = context->t5;
    our_context->t6   = context->t6;
}
