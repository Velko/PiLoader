#						 -*- Mode: makefile -*-

CC=gcc
LD=gcc
OBJS=bootpc.o crc32.o
CFLAGS=-I../common

all: piboot

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

crc32.o: ../common/crc32.c
	$(CC) -c $(CFLAGS) $< -o $@

piboot: $(OBJS)
	$(LD) $(OBJS) -o $@

clean:
	rm -f *.o *~ piboot

