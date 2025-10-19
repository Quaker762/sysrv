/****************************************************************
 * @file    panic.h
 * @brief   Kernel Panic implementation
 *
 * @copyright SPDX-License-Identifier: GPL-3.0-or-later
 ****************************************************************/

#ifndef PANIC_H
#define PANIC_H

/**
 * @brief Panic the Kernel and hang indefinitely
 *
 * @param[in] cause The cause message to print along with the panic string
 */
[[noreturn]] void srv_KernelPanic(const char* cause);

#endif
