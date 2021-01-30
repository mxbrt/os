#include <cpu.h>
#include <dbgu.h>
#include <panic.h>
#include <stdio.h>
#include <string.h>
#include <syscall.h>
#include <thread.h>

// Threads
void echo_sleep(char c) {
  for (int i = 0; i < 10; i++) {
    printf("%c", c);
    syscall_sleep(100);
  }
}

void echo_busywait(char c) {
  for (int i = 0; i < 10; i++) {
    printf("%c", c);
    for (int j = 0; j < 30000000; j++) {};
  }
}

// Threads
void shell(void) {
  while (1) {
    char c = syscall_dbgu_read();
    if (c >= 'A' && c <= 'Z') {
      // uppercase
      syscall_thread_start(&echo_busywait, c);
    }
    if (c >= 'a' && c <= 'z') {
      // lowercase
      syscall_thread_start(&echo_sleep, c);
    }
  }
}

void kernel(void) {
  init();
  scheduler_init();
  // the scheduler is called from the interrupt context, wait for first tick
  thread_start(&shell, 0);
  enable_interrupts();
  while(1) {}
}
