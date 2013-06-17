#include "raspihw.h"
#include "bcm2835.h"

void uart_init()
{
    volatile unsigned i;
    AUX_ENABLES = 1;    // AUX enable
    AUX_MU_IER_REG = 0; // interrupt enable = 0
    AUX_MU_CNTL_REG = 0;// extra control
    AUX_MU_LCR_REG = 3; // line control
    AUX_MU_MCR_REG = 0; // modem control
    AUX_MU_IER_REG = 0; // interrupt enable = 0 (again???)
    AUX_MU_IIR_REG = 0xC6; // interrupt identify
    AUX_MU_BAUD_REG = 270; // speed


    GPFSEL1 = (GPFSEL1 & ~(7<<FSEL14OFF)) | (2<<FSEL14OFF);
    GPFSEL1 = (GPFSEL1 & ~(7<<FSEL15OFF)) | (2<<FSEL15OFF);

    GPPUD = 0;
    for (i=0; i < 150; i++);
    GPPUDCLK0 = (1 << 14) | (1 << 15);
    for (i=0; i < 150; i++);
    GPPUDCLK0 = 0;

    AUX_MU_CNTL_REG = 3;
}

char uart_getc()
{
    /* Wait until character has arrived */
    while ((AUX_MU_LSR_REG & (1 << 0)) == 0);

    /* and return it to caller */
    return AUX_MU_IO_REG;
}

void uart_putc(char c)
{
    /* Wait until the serial buffer is ready */
    while ((AUX_MU_LSR_REG & (1 << 5)) == 0);

    /* Put character into the serial buffer */
    AUX_MU_IO_REG = c;
}
