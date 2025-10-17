/****************************************************************
 * @file    sv39_vm.c
 * @brief   RV64 Sv39 Virtual Memory Functions
 *
 * @copyright SPDX-License-Identifier: GPL-3.0-or-later
 ****************************************************************/

#include <hal.h>

#define RV64_PTE_VALID   (1ULL << 0ULL) /**< Sv39 PTE Valid bit */
#define RV64_PTE_READ    (1ULL << 1ULL) /**< Sv39 PTE Read bit */
#define RV64_PTE_WRITE   (1ULL << 2ULL) /**< Sv39 PTE Write bit */
#define RV64_PTE_EXECUTE (1ULL << 3ULL) /**< Sv39 PTE Execute bit */
#define RV64_PTE_DIRTY   (1ULL << 7ULL) /**< Sv39 PTE Dirty flag */

void srv_hal_MarkPTEValid(page_table_entry_t* pte)
{
    *pte |= RV64_PTE_VALID;
}

void srv_hal_MarkPTEReadable(page_table_entry_t* pte)
{
    *pte |= RV64_PTE_WRITE;
}

void srv_hal_MarkPTEWritable(page_table_entry_t* pte)
{
    *pte |= RV64_PTE_WRITE;
}

void srv_hal_MarkPTEExecutable(page_table_entry_t* pte)
{
    *pte |= RV64_PTE_EXECUTE;
}

bool srv_hal_IsPTEDirty(page_table_entry_t* pte)
{
    return (*pte & RV64_PTE_DIRTY) != 0ULL;
}
