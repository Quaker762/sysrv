/****************************************************************
 * @file    spinlock.c
 * @brief
 *
 * @copyright SPDX-License-Identifier: GPL-3.0-or-later
 ****************************************************************/

#include <lock/spinlock.h>
#include <panic.h>

#include <hal.h>

#include <stddef.h>

void spinlock_InitAnonymous(spinlock_t* const spinlock)
{
    atomic_flag_clear_explicit(&spinlock->lock, memory_order_relaxed);

    spinlock->name = NULL;
    spinlock->cpu  = -1;
}

void spinlock_Acquire(spinlock_t* const spinlock)
{
    while (atomic_flag_test_and_set_explicit(&spinlock->lock, memory_order_acquire))
    {
    }

    /* Okay, we have the lock! */
    spinlock->cpu = (int32_t)srv_hal_GetExecutingCPU();
}

void spinlock_Release(spinlock_t* const spinlock)
{
    /* Get the CPU ID of the processor that called this function (please excuse the unsigned->signed conversion) */
    const int32_t this_cpu = (int32_t)srv_hal_GetExecutingCPU();

    /* Make sure the owner is one trying to unlock it! */
    if (this_cpu != spinlock->cpu)
    {
        srv_KernelPanic("this_cpu != spinlock->cpu!");
    }

    atomic_flag_clear_explicit(&spinlock->lock, memory_order_release);
    spinlock->cpu = -1; /* Nobody owns this now */
}
