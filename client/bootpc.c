#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <elf.h>
#include <termios.h>
#include <string.h>
#include <getopt.h>
#include <stdbool.h>
#include "bootproto.h"

uint32_t crc32(uint32_t crc, const void *buf, size_t size);

FILE *ufile;
Elf32_Ehdr e_hdr;
int ttyfd;
FILE *ttyfs;

void fail_format()
{
    fprintf(stderr,
            "File expected to be: ELF 32-bit LSB executable,\n"
            "ARM, version 1 (SYSV)\n");
    exit(1);
}


void validate_elf()
{
    if (memcmp(e_hdr.e_ident, ELFMAG, SELFMAG) != 0)
        fail_format();

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

void check_response()
{
    struct bp_rsp rsp;
    int res = fread(&rsp, sizeof(rsp), 1, ttyfs);

    if (res != 1) {
        perror("Failed to receive\n");
        exit(1);
    }

    switch (rsp.code) {
    case BPR_ACK:
        printf("OK\n");
        return;
    case BPR_ERR:
        printf("ERR\n");
        break;
    default:
        printf("WTF\n");
        break;
    }
    exit(1);
}

void init_hdr(struct bp_hdr *phdr, uint32_t p_type)
{
    memset(phdr, 0, sizeof(struct bp_hdr));
    phdr->p_type = p_type;
}

void usage()
{
    printf("USAGE:\n");
    printf("    piboot <options> <kernel.elf>\n");
    printf("\n");
    printf("    -h | --help           show this screen\n");
    printf("    -p | --port <device>  use specified serial port to communicate\n");
    printf("    -b | --beef-bss       fill BSS section with 0xDEADBEEF instead of zeros\n");
    printf("    -m | --monitor        start monitoring uart output after loading\n");
}

void load_section(Elf32_Shdr *shdr)
{
    struct bp_hdr phdr;
    void *sdata = malloc(shdr->sh_size);
    if (sdata == NULL) {
        fprintf(stderr, "Out of memory or something.\n");
        exit (1);
    }
    fseek(ufile, shdr->sh_offset, SEEK_SET);
    fread(sdata, shdr->sh_size, 1, ufile);

    init_hdr(&phdr, BPT_LOAD);
    phdr.address = shdr->sh_addr & 0xfffffff;
    phdr.size = shdr->sh_size;
    phdr.crc32 = crc32(0, sdata, shdr->sh_size);

    write(ttyfd, &phdr, sizeof(phdr));
    write(ttyfd, sdata, shdr->sh_size);


    printf("LOAD %08x %08x %08x...", phdr.address, shdr->sh_offset,
	   phdr.size);
    fflush(stdout);

    free(sdata);

    check_response();
}

void zero_section(Elf32_Shdr *shdr)
{
    struct bp_hdr phdr;
    init_hdr(&phdr, BPT_ZERO);
    phdr.address = shdr->sh_addr & 0xfffffff;
    phdr.size = shdr->sh_size;


    write(ttyfd, &phdr, sizeof(phdr));

    printf("ZERO %08x          %08x...", phdr.address, phdr.size);
    fflush(stdout);

    check_response();
}



void beef_section(Elf32_Shdr *shdr)
{
    struct bp_hdr phdr;
    init_hdr(&phdr, BPT_BEEF);
    phdr.address = shdr->sh_addr & 0xfffffff;
    phdr.size = shdr->sh_size;

    write(ttyfd, &phdr, sizeof(phdr));

    printf("BEEF %08x          %08x...", phdr.address, phdr.size);
    fflush(stdout);

    check_response();
}


void exec_program()
{
    struct bp_hdr phdr;
    init_hdr(&phdr, BPT_EXEC);
    phdr.address = e_hdr.e_entry;

    printf("EXEC %08x\n", phdr.address);

    write(ttyfd, &phdr, sizeof(phdr));
}

void ping()
{
    struct bp_hdr phdr;
    struct bp_rsp rsp;

    init_hdr(&phdr, BPT_PING);

    printf("Contacting RasPi bootloader...");
    fflush(stdout);

    write(ttyfd, &phdr, sizeof(phdr));

    fread(&rsp, sizeof(rsp), 1, ttyfs);

    if (rsp.code == BPR_RDY) {
        printf("OK\n");
    } else {
        printf("ERROR\n");
        exit(1);
    }
}



void setup_serial()
{
    struct termios newtio;

    memset(&newtio, 0, sizeof(newtio));
    newtio.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;
    newtio.c_lflag = 0;
    newtio.c_cc[VTIME]    = 0;
    newtio.c_cc[VMIN]     = 1;

    tcflush(ttyfd, TCIFLUSH);
    tcsetattr(ttyfd,TCSANOW,&newtio);

    ttyfs = fdopen(ttyfd, "rb");
}

struct option long_options[] = {
    {"help", no_argument, 0, 'h'},
    {"monitor", no_argument, 0, 'm'},
    {"beef-bss", no_argument, 0, 'b'},
    {"port", required_argument, 0, 'p'},
    {0, 0, 0, 0}
};


void monitor()
{
    char c;
    for (;;) {
        c = fgetc(ttyfs);
        putc(c, stdout);
        fflush(stdout);
    }
}


int main(int argc, char **argv)
{
    int c;
    int option_index = 0;
    bool run_monitor = false;
    bool beef_bss = false;
    char *port = NULL;

    for (;;) {
        c = getopt_long(argc, argv, "hmbp:", long_options, &option_index);
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
        default:
            abort();
        }
    }

    if (optind + 1 != argc || port == NULL) {
        usage();
        exit(1);
    }

    ufile = fopen(argv[optind], "rb");
    if (ufile == NULL) {
        fprintf(stderr, "Can not open file %s\n", argv[optind]);
        return 0;
    }

    fread(&e_hdr, sizeof(e_hdr), 1, ufile);

    validate_elf();

    if (e_hdr.e_shentsize != sizeof(Elf32_Shdr)) {
        fprintf(stderr, "TODO: implement different shentsize\n");
        exit(1);
    }

    ttyfd = open(port, O_RDWR | O_NOCTTY);
    if (ttyfd == -1) {
        fprintf(stderr, "Can not open device %s\n", argv[2]);
        exit(1);
    }

    setup_serial();

    ping();

    Elf32_Shdr *sh_ents = malloc(e_hdr.e_shnum * sizeof(Elf32_Shdr));
    if (sh_ents == NULL) {
        fprintf(stderr, "Out of memory or something\n");
        exit(1);
    }

    fseek(ufile, e_hdr.e_shoff, SEEK_SET);

    fread(sh_ents, sizeof(Elf32_Shdr), e_hdr.e_shnum, ufile);

    unsigned si;
    for (si = 0; si < e_hdr.e_shnum; si++) {
        if (sh_ents[si].sh_flags & SHF_ALLOC) {
            switch (sh_ents[si].sh_type) {
            case SHT_PROGBITS:
                load_section(sh_ents+si);
                break;
            case SHT_NOBITS:
                if (beef_bss)
                    beef_section(sh_ents+si);
                else
                    zero_section(sh_ents+si);
                break;
            default:
                printf("Unknown section\n.");
            }
        }
    }

    free(sh_ents);
    fclose(ufile);

    exec_program();

    if (run_monitor)
        monitor();

    close(ttyfd);
    return 0;
}
