#
# Panic ASM wrapper
#
# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2025 Jesse Buhagiar
#

.type panic, @function
.global panic
.section .text


# Panic entry that captures caller context without a prologue.
# Strategy:
# 1) Save all caller-visible integer registers directly into the caller's
#    stack area (using offsets from `sp`) so nothing is clobbered first.
# 2) Compute the address of the global `panic_frame` and copy the saved
#    words from the stack into that buffer.
# 3) Restore the original `a0` (the `cause` pointer) and call
#    `srv_KernelPanic` (a noreturn C handler).

.align 2
panic:
	/* Save caller registers onto the caller stack (no prologue). */
	sd ra,   0(sp)
	sd sp,   8(sp)
	sd gp,  16(sp)
	sd tp,  24(sp)
	sd t0,  32(sp)
	sd t1,  40(sp)
	sd t2,  48(sp)
	sd s0,  56(sp)
	sd s1,  64(sp)
	sd a0,  72(sp)
	sd a1,  80(sp)
	sd a2,  88(sp)
	sd a3,  96(sp)
	sd a4, 104(sp)
	sd a5, 112(sp)
	sd a6, 120(sp)
	sd a7, 128(sp)
	sd s2, 136(sp)
	sd s3, 144(sp)
	sd s4, 152(sp)
	sd s5, 160(sp)
	sd s6, 168(sp)
	sd s7, 176(sp)
	sd s8, 184(sp)
	sd s9, 192(sp)
	sd s10,200(sp)
	sd s11,208(sp)
	sd t3, 216(sp)
	sd t4, 224(sp)
	sd t5, 232(sp)
	sd t6, 240(sp)

	/* Compute address of global panic_frame into t6. */
	auipc t6, %pcrel_hi(panic_frame)
	addi  t6, t6, %pcrel_lo(panic_frame)

	/* Copy the saved words from caller stack into panic_frame. */
	ld t0,   0(sp); sd t0,   0(t6)
	ld t0,   8(sp); sd t0,   8(t6)
	ld t0,  16(sp); sd t0,  16(t6)
	ld t0,  24(sp); sd t0,  24(t6)
	ld t0,  32(sp); sd t0,  32(t6)
	ld t0,  40(sp); sd t0,  40(t6)
	ld t0,  48(sp); sd t0,  48(t6)
	ld t0,  56(sp); sd t0,  56(t6)
	ld t0,  64(sp); sd t0,  64(t6)
	ld t0,  72(sp); sd t0,  72(t6)
	ld t0,  80(sp); sd t0,  80(t6)
	ld t0,  88(sp); sd t0,  88(t6)
	ld t0,  96(sp); sd t0,  96(t6)
	ld t0, 104(sp); sd t0, 104(t6)
	ld t0, 112(sp); sd t0, 112(t6)
	ld t0, 120(sp); sd t0, 120(t6)
	ld t0, 128(sp); sd t0, 128(t6)
	ld t0, 136(sp); sd t0, 136(t6)
	ld t0, 144(sp); sd t0, 144(t6)
	ld t0, 152(sp); sd t0, 152(t6)
	ld t0, 160(sp); sd t0, 160(t6)
	ld t0, 168(sp); sd t0, 168(t6)
	ld t0, 176(sp); sd t0, 176(t6)
	ld t0, 184(sp); sd t0, 184(t6)
	ld t0, 192(sp); sd t0, 192(t6)
	ld t0, 200(sp); sd t0, 200(t6)
	ld t0, 208(sp); sd t0, 208(t6)
	ld t0, 216(sp); sd t0, 216(t6)
	ld t0, 224(sp); sd t0, 224(t6)
	ld t0, 232(sp); sd t0, 232(t6)
	ld t0, 240(sp); sd t0, 240(t6)

	/* Restore original `a0` (cause pointer) into a0 for the C handler. */
	ld a0, 72(sp)

	/* Call the high-level C panic handler (noreturn). */
	call srv_KernelPanic

	/* Should never return, but loop if it does. */
1:  j 1b

.section .bss
.align 3
.global panic_frame
panic_frame:
.zero 248
.size panic_frame, 248

