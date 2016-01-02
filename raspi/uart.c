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

#include "raspihw.h"
#include "bcm283x.h"

void uart_init()
{
    volatile unsigned i;

    UART0_CR = 0;

    GPFSEL1 = (GPFSEL1 & ~(7<<FSEL14OFF)) | (4<<FSEL14OFF);
    GPFSEL1 = (GPFSEL1 & ~(7<<FSEL15OFF)) | (4<<FSEL15OFF);

    GPPUD = 0;
    for (i=0; i < 150; i++);
    GPPUDCLK0 = (1 << 14) | (1 << 15);
    for (i=0; i < 150; i++);
    GPPUDCLK0 = 0;

    UART0_ICR = 0x7FF;
    UART0_IBRD = 1;
    UART0_FBRD = 40;
    UART0_LCRH = 0x70;
    UART0_CR = 0x301;
}

char uart_getc()
{
    /* Wait until character has arrived */
    while (UART0_FR & UART0_FR_RXFE);

    /* and return it to caller */
    return UART0_DR;
}

void uart_putc(char c)
{
    /* Wait until there's some space in FIFO */
    while (UART0_FR & UART0_FR_TXFF);

    /* Put character there */
    UART0_DR = c;
}

unsigned long uart_drain_rx()
{
    unsigned long t = 0;

    /* Need to read from UART until it becomes empty.

       The tricky part is that we need to read from UART0_DR,
       but have no use for the value. To be sure nothing optimizes
       away, we will calculate some meaningless value and return it.
     */
    while ((UART0_FR & UART0_FR_RXFE) == 0)
        t += UART0_DR;

    return t;
}

void uart_waitidle()
{
    /* Wait until the FIFO is clear and UART has finished with last byte. */
    while ( (UART0_FR & UART0_FR_TXFE ) == 0 || (UART0_FR & UART0_FR_BUSY));
}
