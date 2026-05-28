/****************************************************************
 * @file    exception.c
 * @brief   C Exception Handler implementation
 *
 * @copyright SPDX-License-Identifier: GPL-3.0-or-later
 ****************************************************************/

#include <hal.h>
#include <panic.h>

void srv_arch_ExceptionHandler(void)
{
    srv_cpu_context_t*              context = srv_hal_GetCurrentContextStruct();
    bool                            in_kernel_space;
    const srv_hal_exception_cause_t exception_cause = srv_hal_GetExceptionCause(context, &in_kernel_space);

    /* If the exception happened in the Kernel we need to panic */
    if (in_kernel_space)
    {
        const char* const exception_string = srv_hal_GetExceptionString(exception_cause);
        srv_KernelPanic(exception_string);
    }
}
