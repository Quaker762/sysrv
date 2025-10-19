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
    printf("panic[cpu%d]: %s\n", srv_hal_GetExecutingCPU(), cause);

    /* Spin forever */
    for (;;)
    {
    }
}
