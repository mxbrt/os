#include <cpu.h>
#include <dbgu.h>
#include <stdio.h>
#include <string.h>
#include <thread.h>
#include <panic.h>

void kernel(void) {
  init();
  scheduler_init();
  // the scheduler is called from the interrupt context, wait for first tick
  enable_interrupts();
  while(1) {}
}
