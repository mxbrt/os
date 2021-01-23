#include <panic.h>
#include <syscall.h>

void syscall_exit(void) {
  __asm volatile("svc 0x1");
}

void syscall_sleep(int msec) {
  (void)(msec);
  __asm volatile("svc 0x2");
}
