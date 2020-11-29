#include "string.h"

void *memset(void *s, int c, size_t n) {
  char *dst = (char*)s;
  while (n--) {
    *dst++ = (char)c;
  }
  return s;
}

void *memcpy(void *dest, const void* src, size_t n) {
  char *d = (char*)dest;
  const char *s = (const char*)src;
  while (n--) {
    *d++ = *s++;
  }
  return dest;
}
