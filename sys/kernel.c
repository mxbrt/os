#include <cpu.h>
#include <dbgu.h>
#include <stdio.h>
#include <string.h>

void main(void) {
  printf("Send x to demonstrate printf-formatting\n");
  while (1) {
    char c = (char)getchar();
    printf("Got character: %c\n", c);
    if (c == 'x') {
      const char *str = "Format demo";
      printf("%s: %p %x\n", str, (void*)0xAFFE, 0xAFFE);
    }
    if (c == 'u') {
      printf("Undefined exception: Dereferencing null-pointer\n");
      *(volatile int*)0x0 = 1;
    }
    if (c == 'a') {
      printf("Data abort: Dereferencing invalid memory\n");
      *(volatile int*)0xFFFFFFFF = 1;
    }
    if (c == 's') {
      printf("Emit syscall(0x1)\n");
      __asm volatile("svc 0x1"); \
    }
  }
}

void kernel(void) {
  init();
  main();
}
