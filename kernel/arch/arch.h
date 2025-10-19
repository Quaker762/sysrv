/****************************************************************
 * @file    arch.h
 * @brief   CPU arch indepedent declarations
 *
 * @copyright SPDX-License-Identifier: GPL-3.0-or-later
 ****************************************************************/

#include <hal.h>
#include <stddef.h>

typedef enum
{
    SRV_ARCH_INIT_SUCCESS,             /**< The Arch init routine ran successfully */
    SRC_ARCH_INIT_DEVICE_TREE_INVALID, /**< The arch init routine failed due to a Device Tree parsing error */
} srv_arch_init_result_t;

typedef struct
{
    srv_physical_address_t kernel_physical_address; /**< Physical Load address of the Kernel */
    size_t                 kernel_load_offset;      /**< Offset of the Kernel from the start of physical memory */
    void*                  fdt_ptr;                 /**< Pointer to the Flattened Device Tree structure. May be NULL on some platforms */
} srv_boot_info_t;

/**
 * @brief Architecture Independent initialization routine
 *
 * @details This function brings up the platform to a known state so that the
 *          main kernel program can run without needing to know about what
 *          platform it is being run on
 *
 * @param[in] boot_info Boot Information structure
 *
 * @return @ref SRV_ARCH_INIT_SUCCESS on successfull platform initialization
 */
srv_arch_init_result_t srv_arch_Init(srv_boot_info_t* boot_info);
