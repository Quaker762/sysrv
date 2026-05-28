#
# CPU Trap Handler
#
# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2025 Jesse Buhagiar
#

.type trap_handler, @function
.global trap_handler
.section .text

.extern srv_hal_SaveCPUContext
.extern handle_cpu_trap

.align 4
trap_handler:
    # Save the current CPU context
    call srv_hal_SaveCPUContext

    # Now call the C based trap handler
    call handle_cpu_trap

    sret
