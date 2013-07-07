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
#include "raspihw.h"
#include "bootproto.h"


struct FILE pc_io;
unsigned int saved_r0;
unsigned int saved_r1;
unsigned int saved_r2;

extern uint32_t run_addr;
extern uint32_t run_end;

void start_kernel(unsigned int r0, unsigned int r1, unsigned int r2, void *address) __attribute__((noreturn));

void send_response(uint32_t code, uint32_t data)
{
    struct bp_rsp rsp;
    rsp.code = code;
    rsp.data = data;
    kwrite(&pc_io, &rsp, sizeof(rsp));
}

bool validate_target_addr(uintptr_t addr, size_t size)
{
    uintptr_t run_s = (uintptr_t)&run_addr;
    uintptr_t run_e = (uintptr_t)&run_end;
    uintptr_t addr_e = addr + size;

    run_s -= 4096; // stack (one page should be enough)

    if (run_s <= addr && addr < run_e) return false;
    if (run_s <= addr_e && addr_e < run_e) return false;

    return true;
}

void handle_load(struct bp_hdr *hdr)
{
    if (!validate_target_addr(hdr->address, hdr->size))
    {
        send_response(BPR_ERR, BPE_OVR);
        return;
    }

    kread(&pc_io, (void *)hdr->address, hdr->size);

    uint32_t c_crc = crc32(0, (const void *)hdr->address, hdr->size);

    if (c_crc == hdr->crc32) {
        send_response(BPR_ACK, 0);
    } else {
        send_response(BPR_ERR, BPE_CRC);
    }
}

void handle_zero(struct bp_hdr *hdr)
{
    if (!validate_target_addr(hdr->address, hdr->size))
    {
        send_response(BPR_ERR, BPE_OVR);
        return;
    }

    memset32((void *)hdr->address, (hdr->flags & BPF_BEEF) ? 0xDEADBEEF : 0, hdr->size);
    send_response(BPR_ACK, 0);
}

void handle_exec(struct bp_hdr *hdr)
{
    if ((hdr->flags & BPF_NOWD) == 0)
        wdog_start(0xFFFFF);
    send_response(BPR_ACK, 0);
    uart_waitidle();
    start_kernel(saved_r0, saved_r1, saved_r2, (void *)hdr->address);
}

void handle_unknown(struct bp_hdr *hdr)
{
    /* There's one tricky cause for unknown commands: UART missed
     * a byte for some reason and software now is out of sync from
     * data flow. To make things worse, this error handler is called
     * while out-of-sync data is still coming in.
     *
     * To correct this we should wait a little, so the data flow stops.
     * And then throw it all away. The current operation will, of course,
     * fail but bootloader returns to good state.
     */
    timer_delay(100000); // 0.1 seconds should be enough
    uart_drain_rx();
    send_response(BPR_ERR, hdr->p_type);
}

void pc_io_init()
{
    pc_io.getc = &uart_getc;
    pc_io.putc = &uart_putc;
}

int kmain(unsigned int r0, unsigned int r1, unsigned int r2)
{
    // save Atags, etc if entered
    // for the first time
    if (saved_r1 != 0) {
        saved_r0 = r0;
        saved_r1 = r1;
        saved_r2 = r2;
    }

    okled_init();
    uart_init();
    pc_io_init();
    timer_init();

    wdog_stop();

    struct bp_hdr hdr;

    for (;;) {
        okled_switch(true);
        kread(&pc_io, &hdr, sizeof(hdr));
        okled_switch(false);

        switch (hdr.p_type) {
        case BPT_PING:
            send_response(BPR_RDY, 0);
            break;
        case BPT_LOAD:
            handle_load(&hdr);
            break;
        case BPT_ZERO:
            handle_zero(&hdr);
            break;
        case BPT_EXEC:
            handle_exec(&hdr);
            return 0;
        default:
            handle_unknown(&hdr);
            break;
        }
    }
    return 0;
}
