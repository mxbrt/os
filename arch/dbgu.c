#include "at91rm9200.h"
#include <cpu.h>
#include <dbgu.h>
#include <errno.h>
#include <fifo.h>
#include <stdio.h>

fifo_t rx_fifo = {0};

void init_dbgu(void) {
  AT91C_BASE_DBGU->DBGU_IER = AT91C_US_RXRDY;
}

void dbgu_rx(void) {
  char c = (char)AT91C_BASE_DBGU->DBGU_RHR;
  if(!fifo_put(&rx_fifo, c)) {
    printf("fifo full, dropping %c\n", c);
  }
}

int getchar(void) {
  while (1) {
    char c;

    disable_interrupts();
    if (fifo_get(&rx_fifo, &c)) {
      enable_interrupts();
      return c;
    }
    enable_interrupts();
  }
}

int putchar(int c) {
    AT91C_BASE_DBGU->DBGU_THR = (unsigned int)c;
    return 1;
}

