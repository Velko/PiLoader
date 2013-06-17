#						 -*- Mode: makefile -*-
# Architecture and toolchain path
PREFIX=/usr/local/cross/bin/
ARCH=arm-elf-eabi-

# Compilation flags and additional include directories
ASMFLAGS=-g
CFLAGS=-g -Wall -Werror -ffreestanding -nostdlib -nostartfiles
INCLUDES=-I../common

# Toolchain executables and GCC support library
CC=$(PREFIX)$(ARCH)gcc
ASM=$(PREFIX)$(ARCH)gcc
LD=$(PREFIX)$(ARCH)ld
OBJCOPY=$(PREFIX)$(ARCH)objcopy
GCCLIB:=$(shell $(CC) -print-libgcc-file-name)
LIBS:=

# Search for all .S and .c files. Calculate names of appropriate .o files
ASMFILES := $(wildcard *.S)
CFILES := $(wildcard *.c)
ASMOBJS := $(patsubst %.S,%.o,$(ASMFILES))
COBJS := $(patsubst %.c,%.o,$(CFILES))
EXTRAOBJS := crc32.o

# Default target
all: kernel.img

# Rules how to compile .S and .c files
%.o: %.S
	$(ASM) -c $(ASMFLAGS) $< -o $@

%.o: %.c
	$(CC) -c $(INCLUDES) $(CFLAGS) $< -o $@

crc32.o: ../common/crc32.c
	$(CC) -c $(INCLUDES) $(CFLAGS) $< -o $@
    
# Link object files into kernel executable
kernel.elf: $(ASMOBJS) $(COBJS) $(EXTRAOBJS) link.ld
	$(LD) -n -T link.ld --oformat elf32-littlearm -o $@ \
		$(ASMOBJS) $(COBJS) $(EXTRAOBJS) $(LIBS) $(GCCLIB)

startup.bin: kernel.elf
	$(OBJCOPY) -O binary -j .startup $< $@

# Convert kernel into flat-binary format, to be copied onto SD card
loader.bin: kernel.elf
	$(OBJCOPY) -O binary -R .startup --set-section-flags .bss=alloc,load,contents $< $@

kernel.img: startup.bin loader.bin
	cat startup.bin loader.bin > $@

# Cleanup directory from build files
clean:
	/bin/rm -f *~ *.o kernel.elf kernel.img startup.bin loader.bin
