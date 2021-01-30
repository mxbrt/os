#include <panic.h>
#include <syscall.h>

void syscall_exit(void) {
  __asm volatile("svc 0x1");
}

void syscall_sleep(unsigned int msec) {
  (void)(msec);
  __asm volatile("svc 0x2");
}

void syscall_dbgu_write(char c) {
  (void)(c);
  __asm volatile("svc 0x4");
}
