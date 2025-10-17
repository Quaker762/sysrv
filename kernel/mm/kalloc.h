/**
 * @file    kalloc.h
 * @brief   Kernel Allocation Functions
 *
 * @copyright SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Copyright (C) 2025 Jesse Buhagiar
 */

#ifndef KALLOC_H
#define KALLOC_H

#include <stddef.h>

/**
 * @brief   Allocates data on the eternal kernel heap
 * @details Allocates permanent space for a structure on the eternal kernel heap.
 *          Allocations that occur here can <b>never</b> be freed!
 *
 * @note    This function should be used sparingly for allocations required
 *          during initial bring-up of the Kernel
 */
[[gnu::malloc]] void* srv_kalloc_EternalAlloc(size_t size);

/**
 * @brief   Performs an aligned version of @ref kalloc_EternalAlloc
 *
 * @warning @c alignment must be a power of 2!
 */
[[gnu::malloc]] void* srv_kalloc_EternalAllocAligned(size_t size, size_t alignment);

#endif
