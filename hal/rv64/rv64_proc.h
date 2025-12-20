/****************************************************************
 * @file    rv64_proc.h
 * @brief   RV64 CPU types
 *
 * @copyright SPDX-License-Identifier: GPL-3.0-or-later
 ****************************************************************/

#pragma once

#include <stdint.h>

typedef struct
{
    uint64_t zero; /* x0: hard-wired zero */
    uint64_t ra;   /* x1: return address */
    uint64_t sp;   /* x2: stack pointer */
    uint64_t gp;   /* x3: global pointer */
    uint64_t tp;   /* x4: thread pointer */
    uint64_t t0;   /* x5: temporary */
    uint64_t t1;   /* x6: temporary */
    uint64_t t2;   /* x7: temporary */
    uint64_t s0;   /* x8: saved register / frame pointer */
    uint64_t s1;   /* x9: saved register */
    uint64_t a0;   /* x10: function argument / return value */
    uint64_t a1;   /* x11: function argument / return value */
    uint64_t a2;   /* x12: function argument */
    uint64_t a3;   /* x13: function argument */
    uint64_t a4;   /* x14: function argument */
    uint64_t a5;   /* x15: function argument */
    uint64_t a6;   /* x16: function argument */
    uint64_t a7;   /* x17: function argument */
    uint64_t s2;   /* x18: saved register */
    uint64_t s3;   /* x19: saved register */
    uint64_t s4;   /* x20: saved register */
    uint64_t s5;   /* x21: saved register */
    uint64_t s6;   /* x22: saved register */
    uint64_t s7;   /* x23: saved register */
    uint64_t s8;   /* x24: saved register */
    uint64_t s9;   /* x25: saved register */
    uint64_t s10;  /* x26: saved register */
    uint64_t s11;  /* x27: saved register */
    uint64_t t3;   /* x28: temporary */
    uint64_t t4;   /* x29: temporary */
    uint64_t t5;   /* x30: temporary */
    uint64_t t6;   /* x31: temporary */
} srv_cpu_context_t;

/**
 * @brief Struct describing a logical CPU
 */
typedef struct
{
    srv_cpu_context_t registers; /**< The current register state of this processor */

} srv_cpu_t;
