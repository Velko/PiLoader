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

.section .text
_magic:
.int 0x4c425052 // "magic" to recognize loader by
_start:

    ldr sp, =_magic // Initialize stack to address just below
    bl kmain        // Call kmain() in C
    b .             // Loop forever if returned


// Absolute jump to address
.global start_kernel
start_kernel:
	mov pc, r3

.section .startup, "ax"
.global _startup
_startup:

    // First we need to move everything located
    // after .startup section to running address
    // (currently around 60 MiB address)

    // Do not touch r0-r2 - they contain useful values
    // from GPU loader
    ldr r3, =loaded_text_start
    ldr r4, =loaded_end
    ldr r5, =_magic

_move_loop:
    cmp r3, r4
    beq _move_done
    ldr r6, [r3]
    str r6, [r5]
    add r3, #4
    add r5, #4
    b _move_loop
_move_done:

    // and then jump to it
    ldr pc, =_start
