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

#include "bootpc.h"
#include <elf.h>
#include <stdlib.h>
#include <string.h>

Elf32_Ehdr e_hdr;
bool beef_bss;

static void fail_format()
{
    vm_fail("File expected to be: ELF 32-bit LSB executable,\n"
            "ARM, version 1 (SYSV)\n");
}


static void validate_elf()
{
    if (e_hdr.e_ident[EI_CLASS] != ELFCLASS32)
        fail_format();

    if (e_hdr.e_ident[EI_DATA] != ELFDATA2LSB)
        fail_format();

    if (e_hdr.e_ident[EI_VERSION] != EV_CURRENT)
        fail_format();

    if (e_hdr.e_ident[EI_OSABI] != ELFOSABI_SYSV)
        fail_format();

    if (e_hdr.e_ident[EI_ABIVERSION] != 0)
        fail_format();

    if (e_hdr.e_type != ET_EXEC)
        fail_format();

    if (e_hdr.e_machine != EM_ARM)
        fail_format();
}

bool check_elf()
{
    fseek(ufile, 0, SEEK_SET);
    fread(&e_hdr, sizeof(e_hdr), 1, ufile);
    return memcmp(e_hdr.e_ident, ELFMAG, SELFMAG) == 0;
}


void load_elf(uint32_t *entry_addr)
{
    validate_elf(&e_hdr);

    if (e_hdr.e_phentsize != sizeof(Elf32_Phdr)) {
        vm_fail("TODO: implement different phentsize\n");
    }

    Elf32_Phdr *ph_ents = malloc(e_hdr.e_phnum * sizeof(Elf32_Phdr));
    if (ph_ents == NULL) {
        vm_fail("Out of memory or something\n");
    }

    fseek(ufile, e_hdr.e_phoff, SEEK_SET);
    fread(ph_ents, sizeof(Elf32_Phdr), e_hdr.e_phnum, ufile);

    unsigned pi;
    for (pi = 0; pi < e_hdr.e_phnum; pi++) {

        if (ph_ents[pi].p_type == PT_LOAD) {
            load_section(ph_ents[pi].p_paddr, ph_ents[pi].p_offset, ph_ents[pi].p_filesz);
            uint32_t z_size = ph_ents[pi].p_memsz - ph_ents[pi].p_filesz;
            if (z_size > 0) {
                zero_section(ph_ents[pi].p_paddr + ph_ents[pi].p_filesz, z_size);
            }
        }
    }

    free(ph_ents);

    *entry_addr = e_hdr.e_entry;
}
