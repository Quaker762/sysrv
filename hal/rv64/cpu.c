/****************************************************************
 * @file    cpu.c
 * @brief   CPU related HAL function implementations
 *
 * @copyright SPDX-License-Identifier: GPL-3.0-or-later
 ****************************************************************/

#include <hal.h>

#define RV64_EXCEPTION_CODE_MASK                      0x7FFFFFFFFFFFFFFFULL
#define RV64_SSTATUS_SPP_BIT                          (1ULL << 8)
#define RV64_EXCEPTION_INSTRUCTION_ADDRESS_MISALIGNED 0ULL
#define RV64_EXCEPTION_INSTRUCTION_ACCESS_FAULT       1ULL
#define RV64_EXCEPTION_ILLEGAL_INSTRUCTION            2ULL
#define RV64_EXCEPTION_BREAKPOINT                     3ULL
#define RV64_EXCEPTION_LOAD_ADDRESS_MISALIGNED        4ULL
#define RV64_EXCEPTION_LOAD_ACCESS_FAULT              5ULL
#define RV64_EXCEPTION_STORE_AMO_ADDRESS_MISALIGNED   6ULL
#define RV64_EXCEPTION_STORE_AMO_ACCESS_FAULT         7ULL
#define RV64_EXCEPTION_ECALL_FROM_U_MODE              8ULL
#define RV64_EXCEPTION_ECALL_FROM_S_MODE              9ULL
#define RV64_EXCEPTION_INSTRUCTION_PAGE_FAULT         12ULL
#define RV64_EXCEPTION_LOAD_PAGE_FAULT                13ULL
#define RV64_EXCEPTION_STORE_AMO_PAGE_FAULT           15ULL

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

void srv_hal_InstallTrapHandler(const trap_handler_t* handler)
{
    __asm__ volatile("csrw stvec, %0" ::"r"(handler));
    __asm__ volatile("fence.i" ::
                     : "memory");
}

void hal_SaveContextToProcControlBlock(const srv_cpu_context_t* context)
{
    const uint32_t cpu_num = srv_hal_GetExecutingCPU();

    srv_cpu_context_t* our_context = &cpus[cpu_num].registers;

    /* Copy each register from the provided context into our CPU control block. */
    our_context->zero    = context->zero;
    our_context->ra      = context->ra;
    our_context->sp      = context->sp;
    our_context->gp      = context->gp;
    our_context->tp      = context->tp;
    our_context->t0      = context->t0;
    our_context->t1      = context->t1;
    our_context->t2      = context->t2;
    our_context->s0      = context->s0;
    our_context->s1      = context->s1;
    our_context->a0      = context->a0;
    our_context->a1      = context->a1;
    our_context->a2      = context->a2;
    our_context->a3      = context->a3;
    our_context->a4      = context->a4;
    our_context->a5      = context->a5;
    our_context->a6      = context->a6;
    our_context->a7      = context->a7;
    our_context->s2      = context->s2;
    our_context->s3      = context->s3;
    our_context->s4      = context->s4;
    our_context->s5      = context->s5;
    our_context->s6      = context->s6;
    our_context->s7      = context->s7;
    our_context->s8      = context->s8;
    our_context->s9      = context->s9;
    our_context->s10     = context->s10;
    our_context->s11     = context->s11;
    our_context->t3      = context->t3;
    our_context->t4      = context->t4;
    our_context->t5      = context->t5;
    our_context->t6      = context->t6;
    our_context->sepc    = context->sepc;
    our_context->scause  = context->scause;
    our_context->stval   = context->stval;
    our_context->sstatus = context->sstatus;
}

srv_hal_exception_cause_t srv_hal_GetExceptionCause(const srv_cpu_context_t* const context, bool* was_in_kernel_mode)
{
    const uint64_t cause = (context->scause & RV64_EXCEPTION_CODE_MASK);

    /* Determine if this fault occurred in the Kernel */
    *was_in_kernel_mode = (context->sstatus & RV64_SSTATUS_SPP_BIT) != 0;

    switch (cause)
    {
    case RV64_EXCEPTION_INSTRUCTION_ADDRESS_MISALIGNED:
        return SRV_EXCEPTION_CAUSE_UNALIGNED_INSTRUCTION;
    case RV64_EXCEPTION_ILLEGAL_INSTRUCTION:
        return SRV_EXCEPTION_CAUSE_ILLEGAL_INSTRUCTION;
    case RV64_EXCEPTION_INSTRUCTION_ACCESS_FAULT:
        return SRV_EXCEPTION_CAUSE_INSTRUCTION_ACCESS_FAULT;
    case RV64_EXCEPTION_BREAKPOINT:
        return SRV_EXCEPTION_CAUSE_BREAKPOINT_REACHED;
    case RV64_EXCEPTION_LOAD_ADDRESS_MISALIGNED:
        return SRV_EXCEPTION_CAUSE_UNALIGNED_LOAD;
    case RV64_EXCEPTION_LOAD_ACCESS_FAULT:
        return SRV_EXCEPTION_CAUSE_LOAD_FAULT;
    case RV64_EXCEPTION_STORE_AMO_ADDRESS_MISALIGNED:
        return SRV_EXCEPTION_CAUSE_UNALIGNED_LOAD;
    case RV64_EXCEPTION_STORE_AMO_ACCESS_FAULT:
        return SRV_EXCEPTION_CAUSE_LOAD_FAULT;
    case RV64_EXCEPTION_INSTRUCTION_PAGE_FAULT:
        return SRV_EXCEPTION_CAUSE_PAGE_FAULT;
    case RV64_EXCEPTION_LOAD_PAGE_FAULT:
        return SRV_EXCEPTION_CAUSE_PAGE_FAULT;
    case RV64_EXCEPTION_STORE_AMO_PAGE_FAULT:
        return SRV_EXCEPTION_CAUSE_PAGE_FAULT;
    default:
        return SRV_EXCEPTION_CAUSE_UNKNOWN;
    }
}

char* srv_hal_GetExceptionString(const srv_hal_exception_cause_t cause)
{
    switch (cause)
    {
    case SRV_EXCEPTION_CAUSE_UNALIGNED_INSTRUCTION:
        return "Instruction address misaligned";
    case SRV_EXCEPTION_CAUSE_INSTRUCTION_ACCESS_FAULT:
        return "Instruction access fault";
    case SRV_EXCEPTION_CAUSE_ILLEGAL_INSTRUCTION:
        return "Illegal instruction";
    case SRV_EXCEPTION_CAUSE_BREAKPOINT_REACHED:
        return "Breakpoint";
    case SRV_EXCEPTION_CAUSE_UNALIGNED_LOAD:
        return "Load address misaligned";
    case SRV_EXCEPTION_CAUSE_LOAD_FAULT:
        return "Load access fault";
    case SRV_EXCEPTION_CAUSE_PAGE_FAULT:
        return "Page fault";
    default:
        return "Unknown";
    }
}
