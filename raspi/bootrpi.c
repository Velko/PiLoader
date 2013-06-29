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


void start_kernel(unsigned int r0, unsigned int r1, unsigned int r2, void *address) __attribute__((noreturn));

void send_response(uint32_t code)
{
    struct bp_rsp rsp;
    rsp.code = code;
    kwrite(&pc_io, &rsp, sizeof(rsp));
}

void handle_load(struct bp_hdr *hdr)
{
    kread(&pc_io, (void *)hdr->address, hdr->size);

    if (crc32(0, (const void *)hdr->address, hdr->size) == hdr->crc32) {
        send_response(BPR_ACK);
    } else {
        send_response(BPR_ERR);
    }
}

void handle_zero(struct bp_hdr *hdr)
{
    memset32((void *)hdr->address, (hdr->flags & BPF_BEEF) ? 0xDEADBEEF : 0, hdr->size);
    send_response(BPR_ACK);
}

void handle_exec(struct bp_hdr *hdr)
{
    if ((hdr->flags & BPF_NOWD) == 0)
        wdog_start(0xFFFFF);
    start_kernel(saved_r0, saved_r1, saved_r2, (void *)hdr->address);
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

    wdog_stop();

    struct bp_hdr hdr;

    for (;;) {
        okled_switch(true);
        kread(&pc_io, &hdr, sizeof(hdr));
        okled_switch(false);

        switch (hdr.p_type) {
        case BPT_PING:
            send_response(BPR_RDY);
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
            send_response(BPR_ERR);
            break;
        }
    }
    return 0;
}
