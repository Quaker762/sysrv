/****************************************************************
 * @file    hal.h
 * @brief   SysRV HAL API header
 *
 * @details This header contains function declarations that
 *          <b>must</b> be implemented to support the Kernel
 *          for a given processor platform
 *
 * @copyright SPDX-License-Identifier: GPL-3.0-or-later
 ****************************************************************/

#ifndef HAL_H
#define HAL_H

#include <stdint.h>

typedef uintptr_t srv_physical_address_t; /**< Physical Address type, aliased to @c uintptr_t */
typedef uintptr_t srv_virtual_address_t;  /**< Virtual Address type, aliased to @c uintptr_t */

/**
 * @brief Unmask interrupts from being generated on the processor
 */
void srv_hal_EnableInterrupts(void);

/**
 * @brief Unmask interrupts from being generated on the processor
 */
void srv_hal_DisableInterrupts(void);

#endif
