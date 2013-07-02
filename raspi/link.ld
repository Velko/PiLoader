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

ENTRY(_startup)
load_addr = 0x8000;
run_addr = 0x3c00000;
SECTIONS
{
    . = load_addr;
    .startup :
    {
        *(.startup)
    }
    . = run_addr;
    .text :
    {
        *(.text*)
    }
    .rodata :
    {
       *(.rodata*)
    }
    .data :
    {
        *(.data*)
    }
    .bss :
    {
        *(.bss*)
    }
    loaded_text_start = load_addr + SIZEOF(.startup);
    loaded_end = loaded_text_start + SIZEOF(.text) + SIZEOF(.rodata) + SIZEOF(.data) + SIZEOF(.bss);
}
