#include "raspihw.h"
#include "bcm2835.h"

void okled_init()
{
    /* OK LED is connected to pin GPIO16.

       Will configure it as an output (set FSEL16
       bits to 001 - output).
     */
    GPFSEL1 = (GPFSEL1 & ~(7<<FSEL16OFF)) | 1<<FSEL16OFF;
    GPFSET0 = 1<<GPSET16;
}

void okled_switch(bool on)
{
    if (on) {
        GPFCLR0 = 1<<GPCLR16;
    } else {
        GPFSET0 = 1<<GPSET16;
    }
}
