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

#include "minilibc.h"

int kread(struct FILE *file, void *buffer, int size)
{
    char *b = (char *)buffer;
    int s = size;
    while(s--) {
        *b = file->getc();
        b++;
    }
    return size;
}

int kwrite(struct FILE *file, void *buffer, int size)
{
    char *b = (char *)buffer;
    int s = size;
    while (s--) {
        file->putc(*b);
        b++;
    }
    return size;
}

void *memset32(void *ptr, uint32_t value, size_t bytes)
{
    uint32_t *p = (uint32_t *)ptr;
    while (bytes >= sizeof(uint32_t)) {
        *p = value;
        p++;
        bytes -= sizeof(uint32_t);
    }

    uint8_t *cs = (uint8_t *)&value;
    uint8_t *ct = (uint8_t *)p;
    while (bytes > 0) {
        *ct = *cs;
        ct++;
        cs++;
        bytes--;
    }
    return ptr;
}
