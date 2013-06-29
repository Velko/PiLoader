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
 #include <getopt.h>
 #include <stdlib.h>

struct option long_options[] = {
    {"help", no_argument, 0, 'h'},
    {"monitor", no_argument, 0, 'm'},
    {"beef-bss", no_argument, 0, 'b'},
    {"port", required_argument, 0, 'p'},
    {"verbose", no_argument, 0, 'v'},
    {"load-addr", required_argument, 0, 'l'},
    {"exec-addr", required_argument, 0, 'x'},
    {"suspended", no_argument, 0, 's'},
    {"no-watchdog", no_argument, 0, 'w'},
    {0, 0, 0, 0}
};

static void usage()
{
    printf("USAGE:\n");
    printf("    piboot <options> <kernel>\n");
    printf("\n");
    printf("    -h | --help             Show this screen.\n");
    printf("    -p | --port <device>    Use specified serial port to communicate.\n");
    printf("    -b | --beef-bss         Fill BSS section with 0xDEADBEEF instead of zeros.\n");
    printf("    -m | --monitor          Start monitoring uart output after starting kernel\n");
    printf("    -l | --load-addr <addr> Specify address where to load binary files.\n"
           "       |                    Defaults to 0x8000, increases with each loaded file.\n"
           "       |                    Has no effect when loading ELF files.\n");
    printf("    -x | --exec-addr <addr> Address where to start executing. Defaults to\n"
           "       |                    0x8000. When loading ELF, it's entrypoint address\n"
           "       |                    is used instead.\n");
    printf("    -s | --suspended        Do not start to execute right away.\n");
    printf("    -w | --no-watchdog      Disable watchdog.\n");
    printf("    -v | --verbose          Display what actions are performed.\n");
}

uint32_t parse_addr(const char *s_addr)
{
    char *eptr;
    unsigned long a2 = strtoul(s_addr, &eptr, 16);

    if (*eptr != '\0')
        vm_fail("Not a valid address: '%s'\n", s_addr);

    return a2;
}

void parse_cmdline(int argc, char **argv)
{
    int c;
    int option_index = 0;

    for (;;) {
        c = getopt_long(argc, argv, "hmvbp:l:x:sw", long_options, &option_index);
        if (c == -1)
            break;

        switch (c) {
        case 'h':
            usage();
            exit(0);
        case 'm':
            run_monitor = true;
            break;
        case 'b':
            beef_bss = true;
            break;
        case 'p':
            port = optarg;
            break;
        case 'v':
            verbose_mode = true;
            break;
        case 'l':
            e_load = parse_addr(optarg);
            break;
        case 'x':
            e_entry = parse_addr(optarg);
            break;
        case 's':
            suspended = true;
            break;
        case 'w':
            no_watchdog = true;
            break;
        default:
            abort();
        }
    }

    if (port == NULL) {
        usage();
        exit(1);
    }
}
