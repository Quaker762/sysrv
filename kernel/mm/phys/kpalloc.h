/**
 * @file    kpalloc.h
 * @brief   Kernel Physical Page Allocator
 *
 * @copyright SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Copyright (C) 2025 Jesse Buhagiar
 */

#ifndef KPALLOC_H
#define KPALLOC_H

#include <hal.h>
#include <stddef.h>

#define SRV_PAGE_SIZE 4096ULL /**< The size of a single page */

typedef void* page_t; /**< Physical page typedef */

/**
 * @brief Initialize the physical page allocator
 *
 * @param[in] base_address Physical base address of the memory in the machine
 * @param[in] memory_size  The size of the physical memory installed in the machine (in KiB)
 */
void srv_kpalloc_InitPageAllocator(srv_physical_address_t base_address, size_t memory_size);

/**
 * @brief Allocate a single physical page
 *
 * @return Pointer to the physical address allocated
 */
page_t srv_kpalloc_AllocPage(void);

/**
 * @brief Free a page
 *
 * @param[in] page_ptr Pointer to the physical page address to free
 */
void srv_kpalloc_FreePage(void* page_ptr);

/**
 * @brief Marks a region of memory as unusable
 *
 * @param[in] base_address The base address of the region to mark
 * @param[in] length       The length of the region
 */
void srv_kpalloc_MarkRegionUnusable(srv_physical_address_t base_address, size_t length);

#endif
