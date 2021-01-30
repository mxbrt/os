#include <stdint.h>

#include <dbgu.h>
#include <exception.h>
#include <panic.h>
#include <stdio.h>
#include <string.h>
#include <thread.h>

#include "at91rm9200.h"

void __attribute__((interrupt ("UNDEF"))) undef_handler(void) {
  PANIC("Undefined exception\n");
}

unsigned int *swi_handler(unsigned int regs[16]) {
  (void)(regs);
  char syscall_number = *((char *)(regs[15] - 0x4));
  regs[15] += 0x4;
  // exit
  if (syscall_number == 0x1) {
    return thread_finish();
  // sleep
  } else if (syscall_number == 0x2) {
    scheduler_save_regs(regs);
    return thread_sleep(regs[0]);
  // dbgu read
  } else if (syscall_number == 0x3) {
    char c = 0;
    if (dbgu_getchar(&c)) {
      // save result to r0 and resume thread
      regs[0] = c;
      return regs;
    } else {
      // block thread until a character arrives
      return thread_block(THREAD_BLOCKED_IO_READ, &dbgu_getchar);
    }
  } else if (syscall_number == 0x4) {
    char c = (char)regs[0];
    dbgu_putchar(c);
    return regs;
  } else {
    PANIC("syscall not implemented\n");
  }
}

void __attribute__((interrupt ("ABORT"))) prefetch_abort_handler(void) {
  PANIC("PREFETCH_ABORT NOT IMPLEMENTED\n");
}

void __attribute__((interrupt ("ABORT"))) data_abort_handler(void) {
  int* abort_instruction;
  __asm volatile("mov %0, lr" : "=r" (abort_instruction));
  PANIC("data abort\n");
}

unsigned int *irq_handler(unsigned int regs[16]) {
  (void)(AT91C_BASE_AIC->AIC_IVR);

  unsigned int system_time_status = AT91C_BASE_ST->ST_SR;
  unsigned int *return_regs = regs;
  if (system_time_status) {
    scheduler_save_regs(regs);
    return_regs = scheduler_tick();
    goto out;
  }

  unsigned int dbgu_status = AT91C_BASE_DBGU->DBGU_CSR;
  if (dbgu_status & AT91C_US_RXRDY) {
    dbgu_rx();
    thread_init(&thread_echo);
    goto out;
  }
  PANIC("unknown interrupt");

out:
  AT91C_BASE_AIC->AIC_EOICR = 1;
  return return_regs;
}

void __attribute__((interrupt ("FIQ"))) fiq_handler(void) {
  PANIC("FIQ NOT IMPLEMENTED\n");
}
