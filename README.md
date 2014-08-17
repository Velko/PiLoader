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

Relocates itself
----------------
RasPi firmware loads every kernel from SD card at address 0x8000 and then jumps to same address to start
executing. *PiLoader's* server is no exception. Since this is a second stage bootloader, there's a problem:
things it is meant to load most probably will also want to occupy the space starting from 0x8000. To solve
this, the bootloader first relocates itself to ~60 MiB memory location.


ATags and boot register values
------------------------------
ARM bootloader protocol specifies that registers r0-r2 contains some special values.

* r0 - should be zero;
* r1 - machine type. Could check if running on RasPi or different board;
* r2 - pointer to ATags. A way to telling kernel few important things, like available memory amount;

Since *PiLoader* means to be as transparent as possible, it preserves these values and passes them
to kernels when jumping to them.

Supported files
----------------------
*PiLoader* currently is able to load from plain binary and ELF formats.


Debugging helpers
-----------------
* Supports "polluting" memory areas which normally should be zeroed, with 0xDEADBEEF. Useful to test
  if kernel initializes it's memory correctly. Works only when loading ELF files.


Monitor mode
------------
When kernel is started it will probably start to send some output back using UART. Monitor reads and 
outputs anything that comes from serial line.


Watchdog
--------
RasPi hardware contains a watchdog feature. Once enabled it will wait for certain amount of time
(around 16 s) and then reboot the board. Very useful since we are going to load many untested things
on to RasPi, and they will most likely hang. If not for watchdog, we would need to power cycle
the RasPi then.

If loaded kernel "knows what it is doing" it may disable the watchdog itself. Also there's a
command-line option to do so.


Reentry
-------
If the kernel is finished everything it intended, and if it did not ruin the memory location where
bootloader is located it may simply jump back to the bootloader's entry point.


Intelligent upload
------------------
Kernel file to load sometimes can grow quite large. Often it does not contain any useful code or data
but is just padded with zeros to certain alignment. When loading ELF files, they are already optimized
to exclude such parts. The story is different when loading binary files. They are just a "stream of bytes"
none of which makes any sense to naive loader.

More intelligent loader (which PiLoader is as of recently) can look for big chunks of zeros in binary
streams and upload them using special command to receiver. In other words: instead of sending, say, 16 KiB
worth of zeros, it sends a message saying "and now insert 16 KiB of zeros".


Installation and usage
======================

Clone a Git repo or download and unpack tarball.

Server
------
Server part should be compiled using **ARM cross-compiler** for Raspberry Pi and installed on the SD card.

    cd raspi
    ./configure --host=arm-elf-eabi
    make

Now take a SD card with Raspberry Pi bootloader on it and copy *kernel.img* there. You can re-use a card with
any official Linux distro image (Raspbian for example) if you do not know how to make an empty one. You might 
want to make backup (copy/rename) of existing *kernel.img* there first, if you plan to return to that later.

A note about cross-compiler: yes you need to acquire one. But if you're into low level development, you should
already know that. The recommended way is to [build one](http://velkoraspi.blogspot.com/2012/08/the-toolchain.html).


Client
------
Client part should be compiled a regular compiler on a computer you intend to use.

    cd client
    ./configure
    make
    sudo make install

Connect your Raspi using appropiate serial adapter to computer and power it on.

Run *piboot* specifying port and kernel image to load. For example:

    piboot -p /dev/ttyUSB0 -m kernel.elf


Sample kernel
-------------
Sample kernel is provided to quickly check if everything works. It should be compiled using **ARM cross-compiler**. Similarly
to Server:

    cd samplekernel
    ./configure --host=arm-elf-eabi
    make

Now, try to upload it on Raspberry Pi using *piboot*. It should respond with *"Hello, World!"* and then return to bootloader.


Planned features and updates
============================
* 2-way monitor;
* More samples and documentation;
* ...
