#include <cpu.h>
#include <dbgu.h>
#include <stdio.h>
#include <string.h>
#include <thread.h>
#include <panic.h>

void kernel_thread(void) {
  while (1) {
    // TODO: enable sleep mode
  }
}

void kernel(void) {
  init();
  scheduler_init();
  thread_init(&kernel_thread);
  // the scheduler is called from the interrupt context, wait for first tick
  enable_interrupts();
  while(1) {}
}
