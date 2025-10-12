/****************************************************************
 * @file    string.h
 * @brief   Kernel Standard Library string.h implementation
 *
 * @copyright SPDX-License-Identifier: GPL-3.0-or-later
 ****************************************************************/

#ifndef STRING_H
#define STRING_H

#include <stddef.h>

/**
 * @brief Kernel Standard Library memcpy implementation
 *
 * @param[in] s1 Pointer to the block to copy to
 * @param[in] s2 Pointer to the block to copy from
 * @param[in] n  Number of bytes to copy
 *
 * @return Pointer to @c s1
 */
[[gnu::access(write_only, 1), gnu::access(read_only, 2)]] void* memcpy(void* restrict s1, const char* restrict s2, size_t n);

#endif
