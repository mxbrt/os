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
  }
}
