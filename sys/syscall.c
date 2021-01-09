#include <panic.h>

void syscall_exit(void) {
  __asm volatile("svc 0x1");
}

