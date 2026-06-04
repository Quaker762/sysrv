/****************************************************************
 * @file    panic.c
 * @brief   Implementation of @ref panic.h
 *
 * @copyright SPDX-License-Identifier: GPL-3.0-or-later
 ****************************************************************/

#include <panic.h>

#include <hal.h>
#include <kstdlib/stdio.h>

[[noreturn]] void srv_KernelPanic(const char* cause)
{
    kprintf("panic[cpu%d]: %s\n", srv_hal_GetExecutingCPU(), cause);

    const srv_cpu_context_t* const context = srv_hal_GetCurrentContextStruct();

    /* Do a register dump */
    kprintf("ra: 0x%016lx sp:  0x%016lx gp:  0x%016lx tp: 0x%016lx\n", context->ra, context->sp, context->gp, context->tp);
    kprintf("t0: 0x%016lx t1:  0x%016lx t2:  0x%016lx s0: 0x%016lx\n", context->t0, context->t1, context->t2, context->s0);
    kprintf("s1: 0x%016lx a0:  0x%016lx a1:  0x%016lx a2: 0x%016lx\n", context->s1, context->a0, context->a1, context->a2);
    kprintf("a3: 0x%016lx a4:  0x%016lx a5:  0x%016lx a6: 0x%016lx\n", context->a3, context->a4, context->a5, context->a6);
    kprintf("a7: 0x%016lx s2:  0x%016lx s3:  0x%016lx s4: 0x%016lx\n", context->a7, context->s2, context->s3, context->s4);
    kprintf("s5: 0x%016lx s6:  0x%016lx s7:  0x%016lx s8: 0x%016lx\n", context->s5, context->s6, context->s7, context->s8);
    kprintf("s9: 0x%016lx s10: 0x%016lx s11: 0x%016lx t3: 0x%016lx\n", context->s9, context->s10, context->s11, context->t3);
    kprintf("t4: 0x%016lx t5:  0x%016lx t6:  0x%016lx\n", context->t4, context->t5, context->t6);

    /* Spin forever */
    for (;;)
    {
    }
}
