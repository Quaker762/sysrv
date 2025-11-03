/**
 * @file    kalloc.c
 * @brief   Implementation of @ref kalloc.h
 *
 * @copyright SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Copyright (C) 2025 Jesse Buhagiar
 */

#include <mm/kalloc.h>
#include <hal.h>
#include <panic.h>

extern uintptr_t __kalloc_eternal_start;
extern uintptr_t __kalloc_eternal_end;
static uintptr_t curr_kalloc_eternal_address = (uintptr_t)&__kalloc_eternal_start;

void* srv_kalloc_EternalAlloc(size_t size)
{
    void* alloc_ptr = NULL;

    /* Make sure we aren't don't spill over */
    if ((curr_kalloc_eternal_address + size) < (uintptr_t)&__kalloc_eternal_end)
    {
        /* Allocate it! */
        alloc_ptr = (void*)curr_kalloc_eternal_address;

        /* Bump it! */
        curr_kalloc_eternal_address += size;
    }
    else
    {
        srv_KernelPanic("Eternal Heap Exhausted!");
    }

    return alloc_ptr;
}

[[gnu::malloc]] void* srv_kalloc_EternalAllocAligned(size_t size, size_t alignment)
{
    /* Align the current allocation pointer */
    curr_kalloc_eternal_address = curr_kalloc_eternal_address + (curr_kalloc_eternal_address & (alignment - 1ULL));

    return srv_kalloc_EternalAlloc(size);
}
