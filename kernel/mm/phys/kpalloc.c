/**
 * @file    kpalloc.c
 * @brief   Implementation of @ref physalloc.h
 *
 * @copyright SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Copyright (C) 2025 Jesse Buhagiar
 */

#include <mm/phys/kpalloc.h>
#include <mm/kalloc.h>

typedef uint64_t physalloc_bmap_entry_t;

#define PHYSALLOC_BITS_PER_ENTRY  (sizeof(physalloc_bmap_entry_t) * 8ULL) /**< Number of bits in a bitmap array entry */
#define PHYSALLOC_PAGES_PER_ENTRY PHYSALLOC_BITS_PER_ENTRY                /**< Number of pages in a single bitmap array entry  */
#define PHYSALLOC_NO_FREE_PAGES   UINT64_MAX                              /**< Marker indicating that there are no free pages in this entry */
#define PHYSALLOC_BYTES_PER_ENTRY sizeof(physalloc_bmap_entry_t)          /**< Number of bytes in a bitmap array entry */

/**
 * @brief The number of addressable bytes in a single bitmap entry
 */
#define PHYSALLOC_ALLOC_PER_ENTRY (PHYSALLOC_BITS_PER_ENTRY * PHYSALLOC_PAGE_SIZE)

static physalloc_bmap_entry_t* page_bitmap = NULL;

static uintptr_t free_pages         = 0ULL; /**< Number of memory pages in the system */
static uintptr_t phys_base_address  = 0ULL; /**< Base address of physical memory */
static size_t    bitmap_entry_count = 0ULL;

/**
 * @brief Round a value up to a page size
 *
 * @param[in] value The value to round
 *
 * @return The value rounded to the size of a page
 */
static inline size_t kpalloc_bitmap_RoundToPage(size_t value)
{
    return ((value + SRV_PAGE_SIZE - 1) / SRV_PAGE_SIZE) * SRV_PAGE_SIZE;
}

static inline size_t kpalloc_bitmap_AddressToBitIndex(uintptr_t address)
{
    return (address - phys_base_address) / SRV_PAGE_SIZE;
}

static inline void* kpalloc_bitmap_BitIndexToPageAddress(size_t bit_index)
{
    return (void*)(phys_base_address + (bit_index * SRV_PAGE_SIZE));
}

/**
 * @brief Set a bit in the page bitmap
 *
 * @param[in] bit The bit to set
 */
static inline void kpalloc_bitmap_SetBit(size_t bit)
{
    page_bitmap[bit / PHYSALLOC_BITS_PER_ENTRY] |= (1ULL << (bit % PHYSALLOC_BITS_PER_ENTRY));
}

static inline void kpalloc_bitmap_UnsetBit(size_t bit)
{
    page_bitmap[bit / PHYSALLOC_BITS_PER_ENTRY] &= ~(1ULL << (bit % PHYSALLOC_BITS_PER_ENTRY));
}

static inline bool kpalloc_bitmap_IsBitSet(size_t bit)
{
    const uint64_t mask  = 1ULL << (bit % PHYSALLOC_BITS_PER_ENTRY);
    const uint64_t entry = page_bitmap[bit / PHYSALLOC_BITS_PER_ENTRY];

    return ((mask & entry) != 0ULL);
}

void kpalloc_Init(uintptr_t base_address, size_t memory_size)
{
    phys_base_address = base_address; /* Set the physical base address of all RAM */
    free_pages        = (memory_size / SRV_PAGE_SIZE);

    /* Work out how many uint64_t's there are in our bitmap */
    bitmap_entry_count = (free_pages / PHYSALLOC_PAGES_PER_ENTRY);
    page_bitmap        = srv_kalloc_EternalAlloc(bitmap_entry_count * sizeof(uint64_t));
}

void* kpalloc_AllocPage(void)
{
    void*  page_ptr  = NULL;
    size_t bit_index = 0ULL;

    for (size_t bitmap_index = 0ULL; bitmap_index < bitmap_entry_count; bitmap_index++)
    {
        const physalloc_bmap_entry_t bitmap_entry = page_bitmap[bitmap_index];

        /* If there are no free pages in this entry then go to the next one */
        if (bitmap_entry == PHYSALLOC_NO_FREE_PAGES)
        {
            bit_index += PHYSALLOC_BITS_PER_ENTRY;
            continue;
        }

        /* This entry is completely free! Take bit0 */
        if (bitmap_entry == 0ULL)
        {
            kpalloc_bitmap_SetBit(0ULL);

            /* Construct the page address to return */
            page_ptr = kpalloc_bitmap_BitIndexToPageAddress(bit_index);
            free_pages--;

            break;
        }
        else
        {
            /* Find the first bit that's free */
            const size_t first_bit_free = __builtin_ctzl(~bitmap_entry);
            const size_t page_bit_index = bit_index + first_bit_free;
            kpalloc_bitmap_SetBit(page_bit_index);

            page_ptr = kpalloc_bitmap_BitIndexToPageAddress(page_bit_index);
            free_pages--;

            break;
        }
    }

    return page_ptr;
}

void kpalloc_FreePage(void* page_ptr)
{
    /* Convert the page pointer to a physical address */
    const srv_physical_address_t page_addr = (srv_physical_address_t)page_ptr;

    /* Make sure the address is actually aligned a page boundary */
    if ((page_addr & (SRV_PAGE_SIZE - 1ULL)) != 0ULL)
    {
        /* TODO: We need to kpanic() here */
        return;
    }

    const size_t bit_index = kpalloc_bitmap_AddressToBitIndex(page_addr);
    kpalloc_bitmap_UnsetBit(bit_index);

    free_pages++;
}

void kpalloc_MarkRegionUnusable(srv_physical_address_t base_address, size_t length)
{
    const size_t pages = length / SRV_PAGE_SIZE;

    /* Walk the bitmap and mark each consecutive bit as used */
    size_t curr_bit_index = kpalloc_bitmap_AddressToBitIndex(base_address);
    for (size_t page = 0UL; page < pages; page++)
    {
        /* Yeah, this could probably have been a while loop, but I like bounded loops a bit better */
        (void)page;

        kpalloc_bitmap_SetBit(curr_bit_index);
        curr_bit_index++;
    }
}
