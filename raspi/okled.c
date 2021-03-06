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

#include "raspihw.h"
#include "bcm283x.h"

#ifdef TARGET_BOARD_RPI

void okled_init()
{
    /* Original Raspberry Pi's OK LED is connected to pin GPIO16.

       Will configure it as an output (set FSEL16
       bits to 001 - output).
     */
    GPFSEL1 = (GPFSEL1 & ~(7<<FSEL16OFF)) | 1<<FSEL16OFF;
    GPFSET0 = 1<<GPSET16;
}

void okled_switch(bool on)
{
    if (on) {
        GPFCLR0 = 1<<GPCLR16;
    } else {
        GPFSET0 = 1<<GPSET16;
    }
}

#endif

#ifdef TARGET_BOARD_RPI2

void okled_init()
{
    /* Raspberry Pi 2's OK LED is connected to pin GPIO47.

       Will configure it as an output (set FSEL47
       bits to 001 - output).
     */
    GPFSEL4 = (GPFSEL4 & ~(7<<FSEL47OFF)) | 1<<FSEL47OFF;
    GPFSET0 = 1<<GPSET47;
}

void okled_switch(bool on)
{
    if (on) {
        GPFSET1 = 1<<GPSET47;
    } else {
        GPFCLR1 = 1<<GPCLR47;
    }
}

#endif
