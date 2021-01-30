#include "at91rm9200.h"
#include <cpu.h>
#include <dbgu.h>
#include <fifo.h>
#include <panic.h>
#include <stdio.h>

fifo_t rx_fifo = {
  .buf = {0},
  .rx = 0,
  .tx = 0,
  .cnt = 0,
};

void init_dbgu(void) {
  AT91C_BASE_DBGU->DBGU_IER = AT91C_US_RXRDY;
}

void dbgu_rx(void) {
  char c = (char)AT91C_BASE_DBGU->DBGU_RHR;
  if(!fifo_put(&rx_fifo, c)) {
    WARN("dbgu_rx_fifo full.");
  }
}

int dbgu_getchar(char *c) {
  if (fifo_get(&rx_fifo, c)) {
    return 1;
  }
  return 0;
}

int dbgu_putchar(char c) {
  AT91C_BASE_DBGU->DBGU_THR = c;
  return 1;
}

void kmsg(const char *type, const char *msg) {
  while (*type) {
    dbgu_putchar(*type++);
  }
  while (*msg) {
    dbgu_putchar(*msg++);
  }
}
