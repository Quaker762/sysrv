/****************************************************************
 * @file    trap.c
 * @brief   C trap handler implementation
 *
 * @copyright SPDX-License-Identifier: GPL-3.0-or-later
 ****************************************************************/

#include <hal.h>

void handle_cpu_trap(void)
{
    srv_cpu_context_t* context = srv_hal_GetCurrentContextStruct();
    (void)context;

    __asm__ volatile("nop");
}
