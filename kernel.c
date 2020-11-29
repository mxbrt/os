#include "at91rm9200.h"
#include "ivt.h"
#include "stdio.h"
#include "string.h"

void kernel(void) {
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
