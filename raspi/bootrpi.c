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
    memset32((void *)hdr->address, 0, hdr->size);
    send_response(BPR_ACK);
}

void handle_beef(struct bp_hdr *hdr)
{
    memset32((void *)hdr->address, 0xDEADBEEF, hdr->size);
    send_response(BPR_ACK);
}

void handle_exec(struct bp_hdr *hdr)
{
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
        case BPT_BEEF:
            handle_beef(&hdr);
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
