#include <dbgu.h>

#include "at91rm9200.h"

int getchar(void) {
  while (!(AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_RXRDY)) {};
  return (int)AT91C_BASE_DBGU->DBGU_RHR;
}

int putchar(int c) {
    AT91C_BASE_DBGU->DBGU_THR = (unsigned int)c;
    return 1;
}

