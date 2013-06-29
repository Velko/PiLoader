#						 -*- Mode: makefile -*-
# Copyright (c) 2013 Jurģis Brigmanis
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

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
EXTRAOBJS :=

# Default target
all: kernel.img

# Rules how to compile .S and .c files
%.o: %.S
	$(ASM) -c $(ASMFLAGS) $< -o $@

%.o: %.c
	$(CC) -c $(INCLUDES) $(CFLAGS) $< -o $@

# Link object files into kernel executable
kernel.elf: $(ASMOBJS) $(COBJS) $(EXTRAOBJS) link.ld
	$(LD) -n -T link.ld --oformat elf32-littlearm -o $@ \
		$(ASMOBJS) $(COBJS) $(EXTRAOBJS) $(LIBS) $(GCCLIB)

# Convert kernel into flat-binary format, to be copied onto SD card
kernel.img: kernel.elf
	$(OBJCOPY) -O binary --set-section-flags .bss=alloc,load,contents $< $@


# Cleanup directory from build files
clean:
	/bin/rm -f *~ *.o kernel.elf kernel.img
