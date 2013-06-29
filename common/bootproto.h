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

#ifndef _BOOTPROTO_H_
#define _BOOTPROTO_H_

#include <stdint.h>

/* Bootloader protocol:
	* RasPi bootloader starts and waits for data on serial connection
	* PC loader sends PING packet to check if RasPi loader is there
	* RasPi loader responds with RDY
	* PC loader sends data using LOAD or ZERO packets
	* RasPi loader responds with ACK or ERR
	* PC loader sends EXEC. RasPi starts executing loaded program.

	Note: if loaded program does not trash memory where bootloader
	resides, it may return to it when done.


  PC -> RasPi packets, always size of bp_hdr:
	* PING - fields other than <p_type> ignored
	* LOAD - <size> bytes to be loaded at <address> follows.
		Loaded data must have <crc32> checksum.
	* ZERO - <size> bytes at <address> must be zeroed.
        * BEEF - <size> bytes at <address> must be filled with 0xDEADBEEF.
	* EXEC - start executing program at <address>
*/


/* Packet sent from PC to RasPi */
struct bp_hdr {
    uint32_t p_type;  /* type of packet, see below */
    uint32_t address; /* address to be loaded at */
    uint32_t size;    /* amount of bytes to be loaded */
    uint32_t crc32;   /* checksum of data (PROGBITS only) */
    uint32_t flags;   /* flags for operation */
} __attribute__((packed));

#define BPT_PING    0x474e4950  /* Check if bootloader is alive */
#define BPT_LOAD    0x44414f4c  /* Load program/data     */
#define BPT_ZERO    0x4f52455a  /* Zero memory area      */
#define BPT_EXEC    0x43455845  /* Bootloader done, start program */
#define BPT_BEEF    0x46454542  /* Fill memory area with 0xDEADBEEF */


#define BPF_NOWD    0x00000001  /* Disable watchdog while executing */

/* Response from RasPi to PC */
struct bp_rsp {
    uint32_t code; /* response code, see below */
};

#define BPR_RDY       0x0a594452  /* ready to receive data */
#define BPR_ACK       0x0a4b4341  /* packet received and verified */
#define BPR_ERR       0x0a525245  /* packet received but failed */


#define BLD_MAGIC     0x4c425052  /* bootloader "magic" */

#endif
