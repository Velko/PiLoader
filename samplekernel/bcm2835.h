/* Copyright (c) 2013 Jurģis Brigmanis
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

#ifndef _BCM2835_H_
#define _BCM2835_H_

#include <stdint.h>

/* Base macros for accessing special function registers. */
#define _SFR_IO32(io_addr)  (*((volatile unsigned long *)(io_addr)))

#define BCM2835_IO_BASE     0x20000000

/* Defines to access GPIO registers. */
#define BCM2835_GPIO_BASE   BCM2835_IO_BASE + 0x00200000

#define GPFSEL1    _SFR_IO32(BCM2835_GPIO_BASE + 0x04)
#define GPFSET0    _SFR_IO32(BCM2835_GPIO_BASE + 0x1C)
#define GPFCLR0    _SFR_IO32(BCM2835_GPIO_BASE + 0x28)

#define GPPUD       _SFR_IO32(BCM2835_GPIO_BASE + 0x94)
#define GPPUDCLK0   _SFR_IO32(BCM2835_GPIO_BASE + 0x98)

#define FSEL14OFF  12
#define FSEL15OFF  15
#define FSEL16OFF  18
#define GPSET16    16
#define GPCLR16    16


/* AUX (mini uart, etc) */
#define BCM2835_AUX_BASE   BCM2835_IO_BASE + 0x00215000

#define AUX_ENABLES     _SFR_IO32(BCM2835_AUX_BASE + 0x04)
#define AUX_MU_IO_REG   _SFR_IO32(BCM2835_AUX_BASE + 0x40)
#define AUX_MU_IER_REG  _SFR_IO32(BCM2835_AUX_BASE + 0x44)
#define AUX_MU_CNTL_REG _SFR_IO32(BCM2835_AUX_BASE + 0x60)
#define AUX_MU_LCR_REG  _SFR_IO32(BCM2835_AUX_BASE + 0x4C)
#define AUX_MU_MCR_REG  _SFR_IO32(BCM2835_AUX_BASE + 0x50)
#define AUX_MU_IIR_REG  _SFR_IO32(BCM2835_AUX_BASE + 0x48)
#define AUX_MU_BAUD_REG _SFR_IO32(BCM2835_AUX_BASE + 0x68)
#define AUX_MU_LSR_REG  _SFR_IO32(BCM2835_AUX_BASE + 0x54)

/* WatchDog */
#define BCM2835_PM_BASE   BCM2835_IO_BASE + 0x00100000
#define PM_RSTC         _SFR_IO32(BCM2835_PM_BASE + 0x1c)
#define PM_WDOG         _SFR_IO32(BCM2835_PM_BASE + 0x24)

#define PM_PASSWORD         0x5a000000
#define PM_WDOG_TIME_MASK   0x000fffff

#define PM_RSTC_WRCFG_CLR           0xffffffcf
#define PM_RSTC_WRCFG_FULL_RESET    0x00000020
#define PM_RSTC_RESET               0x00000102




#endif // _BCM2835_H_
