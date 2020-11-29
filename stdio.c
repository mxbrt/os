#include "at91rm9200.h"
#include "stdio.h"
#include "string.h"

int getchar(void) {
  while (!(AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_RXRDY)) {};
  return (int)AT91C_BASE_DBGU->DBGU_RHR;
}

int putchar(int c) {
    AT91C_BASE_DBGU->DBGU_THR = (unsigned int)c;
    return 1;
}

int puts(const char *s) {
  int cnt = 0;
  while (*s) {
    putchar(*s++);
    cnt++;
  }
  return cnt;
}

void print_hex(unsigned int x, int pad) {
  char str[8];
  memset(str, '0', sizeof(str));

  int i = sizeof(str) - 1;
  while (x) {
    unsigned int c = x % 16;
    x = x / 16;
    if (c < 10) {
      c += '0';
    } else {
      c += 'A' - 10;
    }
    str[i--] = (char)c;
  }

  int start = pad ? 0 : i+(i!=sizeof(str)-1);
  for (int j = start; j < 8; j++) {
    putchar(str[j]);
  }
}

int printf(const char *format, ...) {
  va_list ap;
  int n = 0; // length of formatted string

  va_start(ap, format);
  const char *c = format;
  while (*c) {
    const char format_char = *(c+1);
    if (*c == '%' && format_char) {
      switch (format_char) {
        case 'c':
          putchar((int)(unsigned char)va_arg(ap, int));
          break;
        case 'p':
          puts("0x");
          print_hex((unsigned int)va_arg(ap, void*), 1);
          break;
        case 's':
          puts(va_arg(ap, const char *));
          break;
        case 'x':
          print_hex((unsigned int)va_arg(ap, void*), 0);
          break;
        default:
          // print unknown format parameters
          putchar(*c);
          putchar(*c+1);
          break;
      }
      c += 2;
    } else {
      putchar(*c++);
    }
  }
  va_end(ap);
  return n;
}
