/****************************************************************
 * @file    string.c
 * @brief   Implementation of @ref string.h
 *
 * @copyright SPDX-License-Identifier: GPL-3.0-or-later
 ****************************************************************/

#include <string.h>

#include <stdint.h>

void* memcpy(void* restrict s1, const void* restrict s2, size_t n)
{
    uint8_t*       s1_as_u8 = (uint8_t*)s1;
    const uint8_t* s2_as_u8 = (const uint8_t*)s2;

    /* This is _really_ naive an unoptimized, but it doesn't really matter for now! */
    for (size_t i = 0; i < n; i++)
    {
        s1_as_u8[i] = s2_as_u8[i];
    }

    return s1;
}

size_t strlen(const char* str)
{
    size_t count = 0ULL;

    while (str[count] != '\0')
    {
        count++;
    }

    return count;
}

int strcmp(const char* s1, const char* s2)
{
    size_t index = 0ULL;

    char s1_char = s1[index];
    char s2_char = s2[index];
    while (s1_char != '\0' && (s1_char == s2_char))
    {
        index++;
        s1_char = s1[index];
        s2_char = s2[index];
    }

    return (int)s1_char - (int)s2_char;
}
