#include "at91rm9200.h"
#include "exception.h"
#include "stdio.h"
#include "string.h"

void memory_remap(void) {
  AT91C_BASE_MC->MC_RCR = AT91C_MC_RCB;
}

void kernel(void) {
  memory_remap();
  memcpy((void*)0x4, ivt, sizeof(ivt));

  printf("Send x to demonstrate printf-formatting\n");
  while (1) {
    char c = (char)getchar();
    printf("Got character: %c\n", c);
    if (c == 'x') {
      const char *str = "Format demo";
      printf("%s: %p %x\n", str, (void*)0xAFFE, 0xAFFE);
    }
  }
}
