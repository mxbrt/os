#include <cpu.h>
#include <dbgu.h>
#include <stdio.h>
#include <string.h>

void calculation(void) {
    char c = 'A';
    for (int i = 0; i < 300; i++) {
      printf("%c", c);
      for (int j = 0; j < 3000000; j++) {};
      if ((i+1) % 100 == 0) c += 1;
      if ((i+1) % 20 == 0) printf("\n");
    }
    printf("\n");
}

void main(void) {
  while (1) {
    calculation();
    char c = (char)getchar();
    printf("Got character: %c\n", c);
  }
}

void kernel(void) {
  init();
  main();
}
