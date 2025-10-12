/****************************************************************
 * @file    sbicall.h
 * @brief   RV64 Supervisor Binary Interface (SBI) functions
 *
 * @copyright SPDX-License-Identifier: GPL-3.0-or-later
 ****************************************************************/

#ifndef SBICALL_H
#define SBICALL_H

#include <stdint.h>

/**
 * @brief Legacy OpenSBI EIDs
 */
typedef enum
{
    SBICALL_LEGACY_CONSOLE_PUTCHAR = 0x01U /**< Console Putchar extension ID */
} rv64_sbicall_legacy_eid_t;

/**
 * @brief Perform a legacy SBI ECALL with 1 argument
 *
 * @param[in] arg0 The argument to the SBI Call
 * @param[in] eid  Extension ID from @ref rv64_sbicall_legacy_eid_t
 *
 * @return SBI Call specific return code
 */
long sbicall_LegacyEcall1(uintptr_t arg0, rv64_sbicall_legacy_eid_t eid);

#endif
