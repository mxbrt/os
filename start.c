#include "at91rm9200.h"
#include <stddef.h>

void *memcpy(void *dest, const void *src, size_t n)
{
  unsigned char *d = dest;
  const unsigned char *s = src;
  while(n--) {
      *d++ = *s++;
  }
  return dest;
}

__attribute__ ((section (".init")))
void _start(void) {
  while (1) {
    while (!(AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_RXRDY)) {};
    unsigned int x = AT91C_BASE_DBGU->DBGU_RHR;
    AT91C_BASE_DBGU->DBGU_THR = x;
  }
}
