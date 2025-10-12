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

#if defined(__SYSRV_ARCH_RV64__)
#include <rv64/paging.h>
#endif

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

/**
 * @brief Mark a page table entry as valid
 *
 * @param[in] pte Pointer to the Page Table Entry
 */
void srv_hal_MarkPTEValid(page_table_entry_t* pte);

/**
 * @brief Mark a Page Table Entry as readable
 *
 * @param[in] pte Pointer to the Page Table Entry
 */
void srv_hal_MarkPTEReadable(page_table_entry_t* pte);

/**
 * @brief Marks a Page Table Entry as writable
 *
 * @param[in] pte Pointer to the Page Table Entry
 */
void srv_hal_MarkPTEWritable(page_table_entry_t* pte);

/**
 * @brief Marks a Page Table Entry as executable
 *
 * @param[in] pte Pointer to the Page Table Entry
 */
void srv_hal_MarkPTEExecutable(page_table_entry_t* pte);

/**
 * @brief Gets the Dirty status of a Page Table Entry
 *
 * @param[in] pte Pointer to the Page Table Entry
 *
 * @return @c true  if the PTE's "dirty" flag is set
 * @return @c false if the PTE's "dirty" flag is unset
 */
bool srv_hal_IsPTEDirty(page_table_entry_t* pte);

#endif
