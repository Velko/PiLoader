PiLoader
========

A second stage bootloader for the Raspberry Pi to load kernels over UART.

Motive
------
While developing bare-metal software for Raspberry Pi is fun, testing it quickly becomes tiresome.
You need to repeat the same steps over and over again:

1. compile your kernel
2. power down RasPi
3. remove SD card
4. insert card into card reader
5. mount cards filesystem
6. copy compiled kernel to card
7. unmount
8. remove card from the reader
9. put it back into RasPi
10. power on RasPi, check results
11. write some more code
12. repeat

Wouldn't it be nice if the above could be reduced to something like this:

1. compile your kernel
2. run a command, loading it on RasPi
3. check results
4. write some more code
5. repeat

Well, that's what **PiLoader** is all about.


Operation and components
------------------------
Raspberry Pi can be connected to computer in several ways, the easiest (to program) being UART
interface.

Unfortunately, the ease of programming comes with a price - RasPi can not be directly connected
to computer's serial port. It needs some sort of additional hardware to convert incompatible 
voltage levels. I am using my own [homebrew board](http://velkoraspi.blogspot.com/2012/08/building-serial-add-on-board-part-3.html), but I'm sure there are several other options available. 

Once we can send bytes from computer to Raspberry Pi and back, the rest is straightforward. We need 2 pieces of software, speaking the same language at the each end of the connection.

The part sitting on RasPi (second stage bootloader or server) starts the same way as average bare-metal kernel
and sits there waiting for instructions.

The part running on PC (client) checks if it can contact the server over the UART link, and then starts to send
commands and data to be placed in RasPi's memory (hopefully not overwriting the bootloader itself). Once the new
program is loaded, it instructs the server to jump to it.



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