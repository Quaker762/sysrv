#
# Context related assembly functions
#
# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2025 Jesse Buhagiar
#

.type srv_hal_SaveCPUContext, @function
.global srv_hal_SaveCPUContext
.extern hal_SaveContextToProcControlBlock
.section .text

srv_hal_SaveCPUContext:
    # Increase the caller's stack to save the context directly
    addi sp, sp, -256

    # Start saving registers
    sd zero,    0(sp)
	sd ra,      8(sp)
	sd sp,      16(sp)
	sd gp,      24(sp)
	sd tp,      32(sp)
	sd t0,      40(sp)
	sd t1,      48(sp)
	sd t2,      56(sp)
	sd s0,      64(sp)
	sd s1,      72(sp)
	sd a0,      80(sp)
	sd a1,      88(sp)
	sd a2,      96(sp)
	sd a3,      104(sp)
	sd a4,      112(sp)
	sd a5,      120(sp)
	sd a6,      128(sp)
	sd a7,      136(sp)
	sd s2,      144(sp)
	sd s3,      152(sp)
	sd s4,      160(sp)
	sd s5,      168(sp)
	sd s6,      176(sp)
	sd s7,      184(sp)
	sd s8,      192(sp)
	sd s9,      200(sp)
	sd s10,     208(sp)
	sd s11,     216(sp)
	sd t3,      224(sp)
	sd t4,      232(sp)
	sd t5,      240(sp)
	sd t6,      248(sp)

    mv a0, sp

    # Branch to the internal function to actually save it to the control block
    call hal_SaveContextToProcControlBlock

    # Don't leak stack space!
    addi sp, sp, 256
