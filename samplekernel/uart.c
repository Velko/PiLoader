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
#include "bcm2835.h"

void uart_init()
{
    volatile unsigned i;
    AUX_ENABLES = 1;    // AUX enable
    AUX_MU_IER_REG = 0; // interrupt enable = 0
    AUX_MU_CNTL_REG = 0;// extra control
    AUX_MU_LCR_REG = 3; // line control
    AUX_MU_MCR_REG = 0; // modem control
    AUX_MU_IER_REG = 0; // interrupt enable = 0 (again???)
    AUX_MU_IIR_REG = 0xC6; // interrupt identify
    AUX_MU_BAUD_REG = 270; // speed


    GPFSEL1 = (GPFSEL1 & ~(7<<FSEL14OFF)) | (2<<FSEL14OFF);
    GPFSEL1 = (GPFSEL1 & ~(7<<FSEL15OFF)) | (2<<FSEL15OFF);

    GPPUD = 0;
    for (i=0; i < 150; i++);
    GPPUDCLK0 = (1 << 14) | (1 << 15);
    for (i=0; i < 150; i++);
    GPPUDCLK0 = 0;

    AUX_MU_CNTL_REG = 3;
}

void uart_putc(char c)
{
    /* Wait until the serial buffer is ready */
    while ((AUX_MU_LSR_REG & (1 << 5)) == 0);

    /* Put character into the serial buffer */
    AUX_MU_IO_REG = c;
}

void uart_waitidle()
{
    /* Wait until the serial has finished to send. */
    while ((AUX_MU_LSR_REG & (1 << 6)) == 0);
}
