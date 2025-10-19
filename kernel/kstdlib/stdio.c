/****************************************************************
 * @file    stdio.c
 * @brief   Implementation of @c stdio.h
 *
 * @copyright SPDX-License-Identifier: GPL-3.0-or-later
 ****************************************************************/

#include <stdio.h>

#include <hal.h>

static const char* digits = "0123456789ABCDEF";

[[gnu::always_inline]] static inline int printf_internal_Number(int number)
{
    int num = 0;

    if (number < 0)
    {
        srv_hal_WriteDebugChar('-');

        /* Modifying the param directly is bad practice, but let's do it for now */
        number -= number;
    }

    do
    {
        const char to_print  = digits[number % 10];
        number              /= 10;
        srv_hal_WriteDebugChar(to_print);

        num++;
    } while (number != 0);

    return num;
}

[[gnu::always_inline]] static inline int printf_internal_Hex(int number)
{
    /* Stole this from kling, but it's pretty basic */

    int ret    = 0;
    int shifts = 0;
    for (__SIZE_TYPE__ i = number; i > 0; i >>= 4)
    {
        shifts++;
    }

    if (shifts == 0)
    {
        shifts = 1;
    }

    shifts *= 4;
    for (int i = (32 - shifts) / 4; i > 0; i--)
    {
        srv_hal_WriteDebugChar('0');
    }

    while (shifts > 0)
    {
        shifts -= 4;
        srv_hal_WriteDebugChar(digits[(number >> shifts) & 0xFU]);
    }

    return ret;
}

[[gnu::always_inline]] static inline int printf_internal_String(const char* string)
{
    __SIZE_TYPE__ curr_index = 0ULL;
    while (string[curr_index] != '\0')
    {
        srv_hal_WriteDebugChar(string[curr_index]);

        curr_index++;
    }

    return curr_index;
}

int printf_internal(const char* format, __builtin_va_list* va)
{
    __SIZE_TYPE__ index       = 0ULL;
    int           num_written = 0;

    char character = format[index];
    while (character != '\0')
    {
        /* Have we encountered a format specifier? */
        if (character == '%')
        {
            index++;
            const char format_char = format[index];
            switch (format_char)
            {
            case 'c':
            {
                const char c_to_print = __builtin_va_arg(*va, int);
                srv_hal_WriteDebugChar((char)c_to_print);

                num_written++;
                break;
            }
            case 'd':
            {
                int val      = __builtin_va_arg(*va, int);
                num_written += printf_internal_Number(val);

                if (val < 0)
                {
                    num_written++;
                }
                break;
            }
            case 'x':
            {
                uint32_t val  = __builtin_va_arg(*va, uint32_t);
                num_written  += printf_internal_Hex(val);

                break;
            }
            case 's':
            {
                const char* s  = __builtin_va_arg(*va, const char*);
                num_written   += printf_internal_String(s);

                break;
            }
            case '%':
            {
                srv_hal_WriteDebugChar('%');

                num_written++;
                break;
            }
            default:
                break;
            }

            index++;
        }
        else
        {
            /* Just write the character out */
            srv_hal_WriteDebugChar(format[index]);

            num_written++;
            index++;
        }

        character = format[index];
    }

    return num_written;
}

int printf(const char* format, ...)
{
    __builtin_va_list arg_list;

    __builtin_va_start(arg_list, format);
    const int ret = printf_internal(format, &arg_list);
    __builtin_va_end(arg_list);

    return ret;
}
