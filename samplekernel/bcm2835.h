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

#define GPPUD       _SFR_IO32(BCM2835_GPIO_BASE + 0x94)
#define GPPUDCLK0   _SFR_IO32(BCM2835_GPIO_BASE + 0x98)

#define FSEL14OFF  12

/* UART0 */
#define UART0_BASE   BCM2835_IO_BASE + 0x00201000

#define UART0_DR     _SFR_IO32(UART0_BASE+0x00)
#define UART0_FR     _SFR_IO32(UART0_BASE+0x18)
#define UART0_IBRD   _SFR_IO32(UART0_BASE+0x24)
#define UART0_FBRD   _SFR_IO32(UART0_BASE+0x28)
#define UART0_LCRH   _SFR_IO32(UART0_BASE+0x2C)
#define UART0_CR     _SFR_IO32(UART0_BASE+0x30)
#define UART0_ICR    _SFR_IO32(UART0_BASE+0x44)

#define UART0_FR_TXFE    (1 << 7)
#define UART0_FR_TXFF    (1 << 5)
#define UART0_FR_BUSY    (1 << 3)

#endif // _BCM2835_H_
