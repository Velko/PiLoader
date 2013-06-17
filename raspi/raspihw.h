#ifndef _RASPI_HW_H_
#define _RASPI_HW_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

void okled_init();
void okled_switch(bool on);

void uart_init();
void uart_putc(char c);
char uart_getc();
void uart_waitidle();
void uart_set_stdio();

void wdog_start(unsigned int timeout);
unsigned int wdog_get_remaining();
void wdog_stop();


#endif
