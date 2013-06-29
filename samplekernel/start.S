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

.section .startup, "ax"
_start:
    ldr sp, =_start // Initialize stack
    ldr r3, =bss_start
    ldr r4, =bss_end
    mov r5, #0
bss_loop:
    cmp r3, r4
    beq bss_done
    str r5, [r3]
    add r3, #4
    b bss_loop
bss_done:
    bl kmain // Call kmain() in C

    // Check if memory at address 0x3c00000 contains 0x4c425052
    // If so, bootloader should still be there and we may try to return
    // to it.
    mov r0, #0x3c00000
    ldr r1, [r0]
    ldr r2, xb_magic
    cmp r1, r2
    addeq pc, r0, #0x4
    b .     // Loop forever otherwise

xb_magic:
    .int 0x4c425052 // "magic"
