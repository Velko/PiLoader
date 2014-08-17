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

#ifndef _BOOTPC_H_
#define _BOOTPC_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>


void vm_fail(const char *format, ...);
void vm_warn(const char *format, ...);
void vm_print_s(const char *format, ...);
void vm_print_e(bool force_output, const char *format, ...);
uint32_t crc32(uint32_t crc, const void *buf, size_t size);
void monitor();

void ping();
void load_section(uint32_t sh_addr, uint32_t sh_offset, uint32_t sh_size);
void load_buffer(uint32_t sh_addr, void *sdata, uint32_t sh_size);
void zero_section(uint32_t sh_addr, uint32_t sh_size);
void exec_program(uint32_t e_entry);

bool check_elf();
void load_elf(uint32_t *entry_addr);
void load_binary();

void setup_serial(const char *port);

void parse_cmdline(int argc, char **argv);

extern bool verbose_mode;
extern bool beef_bss;
extern FILE *ttyfs;
extern FILE *ufile;
extern int ttyfd;
extern FILE *ttyfs;
extern bool run_monitor;
extern char *port;
extern uint32_t e_entry;
extern uint32_t e_load;
extern bool suspended;
extern bool no_watchdog;

struct bin_part
{
    void *addr;
    size_t size;
};

#endif // _BOOTPC_H_
