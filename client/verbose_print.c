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

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

bool verbose_mode;

char out_buffer[256];
char *out_ptr = out_buffer;

void vm_print_s(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    int nchr = vsprintf(out_ptr, format, ap);
    if (verbose_mode) {
        fputs(out_ptr, stderr);
        fflush(stderr);
    }
    out_ptr += nchr;
    va_end(ap);
}

void vm_print_e(bool force_output, const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    vsprintf(out_ptr, format, ap);
    if (verbose_mode) {
        fputs(out_ptr, stderr);
        fflush(stderr);
    } else if (force_output) {
        fputs(out_buffer, stderr);
        fflush(stderr);
    }
    va_end(ap);
    out_ptr = out_buffer;
}
