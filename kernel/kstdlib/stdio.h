/****************************************************************
 * @file    stdio.h
 * @brief   Kernel Standard Library stdio.h implementation
 *
 * @copyright SPDX-License-Identifier: GPL-3.0-or-later
 ****************************************************************/

#ifndef STDIO_H
#define STDIO_H

/**
 * @brief Kernel internal printf function
 *
 * @param[in] format The format string to write
 *
 * @note This function implicitly writes to the Kernel's debug console using the
 *       underlying Architecture HAL!
 *
 * @return The number of bytes written
 */
[[gnu::format(printf, 1, 2)]] int kprintf(const char* format, ...);

#endif
