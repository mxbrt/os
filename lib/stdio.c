#include <syscall.h>
#include <stdio.h>
#include <string.h>

int puts(const char *s) {
  int cnt = 0;
  while (*s) {
    syscall_dbgu_write(*s++);
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
    syscall_dbgu_write(str[j]);
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
          syscall_dbgu_write((unsigned char)va_arg(ap, int));
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
          syscall_dbgu_write(*c);
          syscall_dbgu_write(*(c+1));
          break;
      }
      c += 2;
    } else {
      syscall_dbgu_write(*c++);
    }
  }
  va_end(ap);
  return n;
}
