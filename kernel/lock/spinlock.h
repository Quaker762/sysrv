/****************************************************************
 * @file    spinlock.h
 * @brief   Architecture Independent Spinlock implementation
 *
 * @copyright SPDX-License-Identifier: GPL-3.0-or-later
 ****************************************************************/

#pragma once

#include <stdint.h>
#include <stdatomic.h>

#define SPINLOCK_LOCKED   0UL
#define SPINLOCK_UNLOCKED 1UL

typedef struct
{
    atomic_flag lock; /**< The atomic value used for the lock */
    int32_t     cpu;  /**< The CPU index that has the lock acquired currently*/
    char*       name; /**< The name of this lock */
} spinlock_t;

/**
 * @brief Initializes an anonymous spinlock
 *
 * @param[in] spinlock The lock to initialize (without a name)
 *
 */
void spinlock_InitAnonymous(spinlock_t* const spinlock);

/**
 * @brief Attempt to acquire the spinlock
 *
 * @param[in] spinlock The spinlock to acquire
 */
void spinlock_Acquire(spinlock_t* const spinlock);

/**
 * @brief Release the spinlock
 *
 * @param[in] spinlock The spinlock to acquire
 *
 * @note This function will panic if the CPU attempting to release the
 *       spinlock is not the same one as the owner
 */
void spinlock_Release(spinlock_t* const spinlock);
