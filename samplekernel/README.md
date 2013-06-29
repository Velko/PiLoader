Sample Kernel
============

This is a sample kernel to try out with the rest of *PiLoader*.

When started it sends back text "Hello, World!" using UART and then
returns the control back to the bootloader.

To try it out build and install everything. Then run:

    ../client/piboot -p /dev/ttyUSB0 -v -m kernel.img

You should see something like this:

    Contacting RasPi bootloader...OK
    LOAD 00008000 00000000 000002f0...OK
    EXEC 00008000
    Hello, World!

Press Ctrl+C to terminate monitor. That's it.
