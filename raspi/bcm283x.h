/* Copyright (c) 2013-2016 Jurģis Brigmanis
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef _BCM283X_H_
#define _BCM283X_H_

#include <stdint.h>
#include "config.h"

/* Base macros for accessing special function registers. */
#define _SFR_IO32(io_addr)  (*((volatile unsigned long *)(io_addr)))

#ifdef TARGET_BOARD_RPI
#define BCM283X_IO_BASE     0x20000000
#endif

#ifdef TARGET_BOARD_RPI2
#define BCM283X_IO_BASE     0x3F000000
#endif

/* Defines to access GPIO registers. */
#define BCM283X_GPIO_BASE   BCM283X_IO_BASE + 0x00200000

#define GPFSEL1    _SFR_IO32(BCM283X_GPIO_BASE + 0x04)
#define GPFSET0    _SFR_IO32(BCM283X_GPIO_BASE + 0x1C)
#define GPFCLR0    _SFR_IO32(BCM283X_GPIO_BASE + 0x28)

#define GPPUD       _SFR_IO32(BCM283X_GPIO_BASE + 0x94)
#define GPPUDCLK0   _SFR_IO32(BCM283X_GPIO_BASE + 0x98)

#define FSEL14OFF  12
#define FSEL15OFF  15
#define FSEL16OFF  18
#define GPSET16    16
#define GPCLR16    16


/* UART0 */
#define UART0_BASE   BCM283X_IO_BASE + 0x00201000

#define UART0_DR     _SFR_IO32(UART0_BASE+0x00)
#define UART0_FR     _SFR_IO32(UART0_BASE+0x18)
#define UART0_IBRD   _SFR_IO32(UART0_BASE+0x24)
#define UART0_FBRD   _SFR_IO32(UART0_BASE+0x28)
#define UART0_LCRH   _SFR_IO32(UART0_BASE+0x2C)
#define UART0_CR     _SFR_IO32(UART0_BASE+0x30)
#define UART0_ICR    _SFR_IO32(UART0_BASE+0x44)

#define UART0_FR_TXFE    (1 << 7)
#define UART0_FR_TXFF    (1 << 5)
#define UART0_FR_RXFE    (1 << 4)
#define UART0_FR_BUSY    (1 << 3)


/* WatchDog */
#define BCM283X_PM_BASE   BCM283X_IO_BASE + 0x00100000
#define PM_RSTC         _SFR_IO32(BCM283X_PM_BASE + 0x1c)
#define PM_WDOG         _SFR_IO32(BCM283X_PM_BASE + 0x24)

#define PM_PASSWORD         0x5a000000
#define PM_WDOG_TIME_MASK   0x000fffff

#define PM_RSTC_WRCFG_CLR           0xffffffcf
#define PM_RSTC_WRCFG_FULL_RESET    0x00000020
#define PM_RSTC_RESET               0x00000102


/* Timer */
#define ARM_TIMER_BASE      BCM283X_IO_BASE + 0x0000B400
#define ARM_TIMER_CTL       _SFR_IO32(ARM_TIMER_BASE + 0x08)
#define ARM_TIMER_CNT       _SFR_IO32(ARM_TIMER_BASE + 0x20)

#endif // _BCM283X_H_
