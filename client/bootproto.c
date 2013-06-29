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

#include <stdlib.h>
#include <string.h>
#include "bootpc.h"
#include "bootproto.h"

static void check_response()
{
    struct bp_rsp rsp;
    int res = fread(&rsp, sizeof(rsp), 1, ttyfs);

    if (res != 1) {
        vm_print_e(true, "Failed to receive\n");
        exit(1);
    }

    switch (rsp.code) {
    case BPR_ACK:
        vm_print_e(false, "OK\n");
        return;
    case BPR_ERR:
        vm_print_e(true, "ERR\n");
        break;
    default:
        vm_print_e(true, "Unknown response\n");
        break;
    }
    exit(1);
}


static void init_hdr(struct bp_hdr *phdr, uint32_t p_type)
{
    memset(phdr, 0, sizeof(struct bp_hdr));
    phdr->p_type = p_type;
}


void ping()
{
    struct bp_hdr phdr;
    struct bp_rsp rsp;

    init_hdr(&phdr, BPT_PING);

    vm_print_s("Contacting RasPi bootloader...");

    write(ttyfd, &phdr, sizeof(phdr));

    fread(&rsp, sizeof(rsp), 1, ttyfs);

    if (rsp.code == BPR_RDY) {
        vm_print_e(false, "OK\n");
    } else {
        vm_print_e(true, "ERROR\n");
        exit(1);
    }
}


void load_section(uint32_t sh_addr, uint32_t sh_offset, uint32_t sh_size)
{
    struct bp_hdr phdr;
    void *sdata = malloc(sh_size);
    if (sdata == NULL) {
        vm_fail("Out of memory or something.\n");
    }
    fseek(ufile, sh_offset, SEEK_SET);
    fread(sdata, sh_size, 1, ufile);

    init_hdr(&phdr, BPT_LOAD);
    phdr.address = sh_addr & 0xfffffff;
    phdr.size = sh_size;
    phdr.crc32 = crc32(0, sdata, sh_size);

    write(ttyfd, &phdr, sizeof(phdr));
    write(ttyfd, sdata, sh_size);

    vm_print_s("LOAD %08x %08x %08x...", phdr.address, sh_offset, phdr.size);

    free(sdata);

    check_response();
}


void zero_section(uint32_t sh_addr, uint32_t sh_size)
{
    struct bp_hdr phdr;
    init_hdr(&phdr, BPT_ZERO);
    phdr.address = sh_addr & 0xfffffff;
    phdr.size = sh_size;

    if (beef_bss)
        phdr.flags |= BPF_BEEF;

    write(ttyfd, &phdr, sizeof(phdr));

    vm_print_s("%s %08x          %08x...", (beef_bss ? "BEEF" : "ZERO" ), phdr.address, phdr.size);

    check_response();
}


void exec_program(uint32_t e_entry)
{
    struct bp_hdr phdr;
    init_hdr(&phdr, BPT_EXEC);
    phdr.address = e_entry;

    if (no_watchdog)
        phdr.flags |= BPF_NOWD;

    vm_print_e(false, "EXEC %08x%s\n", phdr.address, (no_watchdog ? " nwd" : ""));

    write(ttyfd, &phdr, sizeof(phdr));
}
