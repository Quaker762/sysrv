/****************************************************************
 * @file    fdt.h
 * @brief   Flattened Device Tree parser driver
 *
 * @copyright SPDX-License-Identifier: GPL-3.0-or-later
 ****************************************************************/

#include <hal.h>
#include <stdint.h>
#include <stddef.h>

/**
 * @brief FDT Header
 *
 * @sa devicetree-specification.readthedocs.io/en/stable/flattened-format.html
 */
struct fdt_header
{
    uint32_t magic;
    uint32_t totalsize;
    uint32_t off_dt_struct;
    uint32_t off_dt_strings;
    uint32_t off_mem_rsvmap;
    uint32_t version;
    uint32_t last_comp_version;
    uint32_t boot_cpuid_phys;
    uint32_t size_dt_strings;
    uint32_t size_dt_struct;
};

/**
 * @brief Initialize the FDT
 *
 * @param[in] fdt_ptr Pointer to the FDT
 *
 * @warning This function MUST be called before the Virtual Memory subsystem has been configured!
 *
 * @return true     The FDT driver was initialized successfully
 * @return false    An error occurred during initialization, or the FDT provided is invalid
 */
bool srv_fdt_Init(void* fdt_ptr);

/**
 * @brief Get the amount of memory installed in the system
 *
 * @return The amount of memory in the system, in bytes
 */
size_t srv_fdt_GetMemorySize(void);
