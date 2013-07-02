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

#ifndef _MINI_LIBC_H_
#define _MINI_LIBC_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

struct FILE {
    void (*putc)(char c);
    char (*getc)();
};

int kread(struct FILE *file, void *buffer, int size);
int kwrite(struct FILE *file, void *buffer, int size);
void *memset32(void *ptr, uint32_t value, size_t bytes);

uint32_t crc32(uint32_t crc, const void *buf, size_t size);


#endif
