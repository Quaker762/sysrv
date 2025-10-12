/****************************************************************
 * @file    paging.h
 * @brief   RV64 Paging types
 *
 * @copyright SPDX-License-Identifier: GPL-3.0-or-later
 ****************************************************************/

#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>

#define SRV_PAGING_PTE_PER_TABLE 512UL /**< Number of Page Table Entries in a Page Table */

typedef uint64_t           page_table_entry_t;                     /** Page Table Entry typedef */
typedef page_table_entry_t page_table_t[SRV_PAGING_PTE_PER_TABLE]; /** Page Table Typedef */

#endif
