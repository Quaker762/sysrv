/****************************************************************
 * @file    stdio.c
 * @brief   Implementation of @c stdio.h
 *
 * @copyright SPDX-License-Identifier: GPL-3.0-or-later
 ****************************************************************/

#include <stdio.h>

#include <hal.h>
#include <limits.h>

#if (ULONG_MAX == 0xFFFFFFFFUL)
typedef uint32_t srv_printf_ulong_type_t;
#elif ULONG_MAX == 0xFFFFFFFFFFFFFFFFUL
typedef uint64_t srv_printf_ulong_type_t;
#endif

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

[[gnu::always_inline]] static inline int printf_internal_ULong(srv_printf_ulong_type_t number)
{
    srv_printf_ulong_type_t num = 0;

    do
    {
        const char to_print  = digits[number % 10];
        number              /= 10;
        srv_hal_WriteDebugChar(to_print);

        num++;
    } while (number != 0);

    return num;
}

[[gnu::always_inline]] static inline int printf_internal_HexPadded(uint32_t number, int zero_pad_width)
{
    int value_digits = 0;
    for (__SIZE_TYPE__ i = number; i > 0; i >>= 4)
    {
        value_digits++;
    }

    if (value_digits == 0)
    {
        value_digits = 1;
    }

    int digits_to_print = value_digits;
    if (zero_pad_width > digits_to_print)
    {
        digits_to_print = zero_pad_width;
    }

    for (int i = digits_to_print - 1; i >= 0; i--)
    {
        if (i >= value_digits)
        {
            srv_hal_WriteDebugChar('0');
            continue;
        }

        srv_hal_WriteDebugChar(digits[(number >> (i * 4)) & 0xFU]);
    }

    return digits_to_print;
}

[[gnu::always_inline]] static inline int printf_internal_HexULongPadded(srv_printf_ulong_type_t number, int zero_pad_width)
{
    int value_digits = 0;
    for (srv_printf_ulong_type_t i = number; i > 0; i >>= 4)
    {
        value_digits++;
    }

    if (value_digits == 0)
    {
        value_digits = 1;
    }

    int digits_to_print = value_digits;
    if (zero_pad_width > digits_to_print)
    {
        digits_to_print = zero_pad_width;
    }

    for (int i = digits_to_print - 1; i >= 0; i--)
    {
        if (i >= value_digits)
        {
            srv_hal_WriteDebugChar('0');
            continue;
        }

        srv_hal_WriteDebugChar(digits[(number >> (i * 4)) & 0xFU]);
    }

    return digits_to_print;
}

[[gnu::always_inline]] static inline int printf_internal_ParseZeroPadWidth(const char* format, __SIZE_TYPE__* index)
{
    if (format[*index] != '0')
    {
        return 0;
    }

    int zero_pad_width = 0;
    (*index)++;

    while ((format[*index] >= '0') && (format[*index] <= '9'))
    {
        zero_pad_width *= 10;
        zero_pad_width += (int)(format[*index] - '0');
        (*index)++;
    }

    return zero_pad_width;
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

            int zero_pad_width = printf_internal_ParseZeroPadWidth(format, &index);

            const char format_char = format[index];
            switch (format_char)
            {
            case 'l':
            {
                index++;
                const char next_format_char = format[index];

                if (next_format_char == 'x')
                {
                    srv_printf_ulong_type_t val  = __builtin_va_arg(*va, srv_printf_ulong_type_t);
                    num_written                 += printf_internal_HexULongPadded(val, zero_pad_width);
                    break;
                }
                else if (next_format_char == 'u')
                {
                    srv_printf_ulong_type_t val  = __builtin_va_arg(*va, srv_printf_ulong_type_t);
                    num_written                 += printf_internal_ULong(val);
                }

                break;
            }
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
                num_written  += printf_internal_HexPadded(val, zero_pad_width);

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

int kprintf(const char* format, ...)
{
    __builtin_va_list arg_list;

    __builtin_va_start(arg_list, format);
    const int ret = printf_internal(format, &arg_list);
    __builtin_va_end(arg_list);

    return ret;
}

int kvprintf(const char* format, __builtin_va_list va_list)
{
    const int ret = printf_internal(format, &va_list);

    return ret;
}
