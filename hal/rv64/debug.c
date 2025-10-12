/****************************************************************
 * @file    debug.c
 * @brief   Debug related functions for RV64
 *
 * @copyright SPDX-License-Identifier: GPL-3.0-or-later
 ****************************************************************/

#include <hal.h>

#include "sbicall.h"

void srv_hal_WriteDebugChar(char c)
{
    /* Print the character to the console */
    (void)sbicall_LegacyEcall1((uintptr_t)c, SBICALL_LEGACY_CONSOLE_PUTCHAR);
}
