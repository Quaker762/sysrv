/****************************************************************
 * @file    string.c
 * @brief   Implementation of @ref string.h
 *
 * @copyright SPDX-License-Identifier: GPL-3.0-or-later
 ****************************************************************/

#include <string.h>

#include <stdint.h>

void* memcpy(void* restrict s1, const char* restrict s2, size_t n)
{
    uint8_t*       s1_as_u8 = (uint8_t*)s1;
    const uint8_t* s2_as_u8 = (const uint8_t*)s2;

    /* This is _really_ naive an unoptimized, but it doesn't really matter for now! */
    for (size_t i = 0; i < n; i++)
    {
        s1_as_u8[i] = s2_as_u8[n];
    }

    return s1;
}
