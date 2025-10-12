/****************************************************************
 * @file    sbicall.c
 * @brief   Implementation of @ref sbicall.h
 *
 * @copyright SPDX-License-Identifier: GPL-3.0-or-later
 ****************************************************************/

#include "sbicall.h"

long sbicall_LegacyEcall1(uintptr_t arg0, rv64_sbicall_legacy_eid_t eid)
{
    uint32_t a0 = arg0;
    uint32_t a7 = (uint32_t)eid;

    __asm__ volatile(
    "mv a0, %0\n"
    "mv a7, %2\n"
    "ecall\n"
    : "+r"(a0)
    : "r"(a0), "r"(a7)
    : "cc", "memory");

    return a0;
}
