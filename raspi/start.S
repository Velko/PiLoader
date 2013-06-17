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
	mov pc, r0

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
