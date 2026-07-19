/****************************************************************
 * @file    sv39_vm.c
 * @brief   RV64 Sv39 Virtual Memory Functions
 *
 * @copyright SPDX-License-Identifier: GPL-3.0-or-later
 ****************************************************************/

#include <hal.h>

#include "rv64_vm.h"

#define RV64_PTE_VALID   (1ULL << 0ULL) /**< Sv39 PTE Valid bit */
#define RV64_PTE_READ    (1ULL << 1ULL) /**< Sv39 PTE Read bit */
#define RV64_PTE_WRITE   (1ULL << 2ULL) /**< Sv39 PTE Write bit */
#define RV64_PTE_EXECUTE (1ULL << 3ULL) /**< Sv39 PTE Execute bit */
#define RV64_PTE_DIRTY   (1ULL << 7ULL) /**< Sv39 PTE Dirty flag */

static inline uint64_t rv64_GetVPN2(const srv_virtual_address_t va)
{
    return (va >> RV64_SV39_VPN2_BITP) & RV64_SV39_VPN_MASK;
}

static inline uint64_t rv64_GetVPN1(const srv_virtual_address_t va)
{
    return (va >> RV64_SV39_VPN1_BITP) & RV64_SV39_VPN_MASK;
}

static inline uint64_t rv64_GetVPN0(const srv_virtual_address_t va)
{
    return (va >> RV64_SV39_VPN0_BITP) & RV64_SV39_VPN_MASK;
}

static inline srv_physical_address_t rv64_PTEToPhysAddr(const page_table_entry_t* pte)
{
    return (((*pte) >> 10ULL) & 0xFFFFFFFFFFFULL) << 12ULL;
}

void srv_hal_MarkPTEValid(page_table_entry_t* pte)
{
    *pte |= RV64_PTE_VALID;
}

void srv_hal_MarkPTEReadable(page_table_entry_t* pte)
{
    *pte |= RV64_PTE_READ;
}

void srv_hal_MarkPTEWritable(page_table_entry_t* pte)
{
    *pte |= RV64_PTE_WRITE;
}

void srv_hal_MarkPTEExecutable(page_table_entry_t* pte)
{
    *pte |= RV64_PTE_EXECUTE;
}

bool srv_hal_IsPTEDirty(const page_table_entry_t* pte)
{
    return (*pte & RV64_PTE_DIRTY) != 0ULL;
}

bool srv_hal_IsPTEValid(const page_table_entry_t* pte)
{
    return (*pte & RV64_PTE_VALID) != 0ULL;
}

bool srv_hal_IsPTELeaf(const page_table_entry_t* pte)
{
    const bool are_rwx_bits_set = (*pte & (RV64_PTE_READ | RV64_PTE_WRITE | RV64_PTE_EXECUTE)) != 0ULL;
    const bool is_valid         = (*pte & RV64_PTE_VALID) != 0ULL;

    return (are_rwx_bits_set && is_valid);
}

page_table_entry_t* srv_hal_WalkPageTable(const page_table_t* page_table, srv_virtual_address_t va)
{
    page_table_entry_t* found_pte = NULL;

    /* Get the Page Index number for each level */
    const uint64_t vpn[RV64_SV39_MAX_LEVELS] = {rv64_GetVPN0(va), rv64_GetVPN1(va), rv64_GetVPN2(va)};

    const page_table_t* curr_table = page_table;

    /* Start at level 2 and walk the page tables */
    for (size_t level = RV64_SV39_MAX_LEVELS; level > 0; level--)
    {
        const page_table_entry_t* pte = &(*curr_table)[vpn[level - 1ULL]];

        if (srv_hal_IsPTEValid(pte))
        {
            if (srv_hal_IsPTELeaf(pte))
            {
                found_pte = (page_table_entry_t*)pte;
                break;
            }

            curr_table = (page_table_t*)rv64_PTEToPhysAddr(pte);
        }
        else
        {
            break; /* Page is unmapped at this level */
        }
    }

    return found_pte;
}
