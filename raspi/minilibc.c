#include "minilibc.h"

int kread(struct FILE *file, void *buffer, int size)
{
    char *b = (char *)buffer;
    int s = size;
    while(s--) {
        *b = file->getc();
        b++;
    }
    return size;
}

int kwrite(struct FILE *file, void *buffer, int size)
{
    char *b = (char *)buffer;
    int s = size;
    while (s--) {
        file->putc(*b);
        b++;
    }
    return size;
}

void *memset32(void *ptr, uint32_t value, size_t bytes)
{
    uint32_t *p = (uint32_t *)ptr;
    while (bytes >= sizeof(uint32_t)) {
        *p = value;
        p++;
        bytes -= sizeof(uint32_t);
    }

    uint8_t *cs = (uint8_t *)&value;
    uint8_t *ct = (uint8_t *)p;
    while (bytes > 0) {
        *ct = *cs;
        ct++;
        cs++;
        bytes--;
    }
    return ptr;
}
