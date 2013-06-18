PiLoader
========

A second stage bootloader for the Raspberry Pi to load kernels over UART.

Consists of two parts. One to be loaded on Raspberry Pi, and the second to be run on a computer.


Features
========

* Upon start relocates itself to ~60 MiB memory location, freeing up place where to load kernel.
* Preserves boot register values (r0-r3), allowing loaded kernel to utilize information from ATags.
* Loads kernels from ELF file format. 
* Supports "polluting" memory areas which normally should be zeroed, with 0xDEADBEEF. Useful to test
  if kernel initializes it's memory correctly.
* Monitor mode - reads and outputs anything that comes from serial line after kernel is started.
* Initializes chip's watchdog. If not disabled by kernel, it will reboot the Raspi after ~16s.
* When loaded kernel is done, it may restart the bootloader. Requires kernel's cooperation, however.


Installation and usage
======================

Build Raspi part and copy *kernel.img* to the SD card, replacing whatever kernel was there before. You 
might want to make backup first, trough.

Build client part on any Linux computer, copy *piboot* to convenient location.

Connect your Raspi using appropiate serial adapter to computer and power it on.

Run *piboot* specifying port and kernel image to load. For example:

    piboot -p /dev/ttyUSB0 -m kernel.elf

Enjoy!



Planned features and updates
============================
* Plain binary kernel support;
* 2-way monitor;
* Loading using ELF LMA addresses (instead of VMA);
* Option to disable Watchdog;
* More samples and documentation;
* ...