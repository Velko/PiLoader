#ifndef _MINI_LIBC_H_
#define _MINI_LIBC_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

struct FILE {
    void (*putc)(char c);
    char (*getc)();
};

extern struct FILE stdio;

int kread(struct FILE *file, void *buffer, int size);
int kwrite(struct FILE *file, void *buffer, int size);
void *memset32(void *ptr, uint32_t value, size_t bytes);

uint32_t crc32(uint32_t crc, const void *buf, size_t size);


#endif
