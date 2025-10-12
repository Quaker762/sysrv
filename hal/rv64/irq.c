/****************************************************************
 * @file    irq.c
 * @brief   IRQ related HAL functions
 *
 * @copyright SPDX-License-Identifier: GPL-3.0-or-later
 ****************************************************************/

#include <hal.h>

void srv_hal_EnableInterrupts(void)
{
    /* Set SSTATUS.SIE to 1 */
    __asm__ volatile("csrrsi zero, sstatus, 1");
}

void srv_hal_DisableInterrupts(void)
{
    /* Clear SSTATUS.SIE */
    __asm__ volatile("csrrci zero, sstatus, 1");
}
