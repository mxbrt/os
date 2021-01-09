#include <stdint.h>

#include <dbgu.h>
#include <exception.h>
#include <panic.h>
#include <stdio.h>
#include <string.h>
#include <thread.h>

#include "at91rm9200.h"

void __attribute__((interrupt ("UNDEF"))) undef_handler(void) {
  int* undef_instruction;
  __asm volatile("mov %0, lr" : "=r" (undef_instruction));
  PANIC("Undefined exception at %p\n", undef_instruction - 1);
}

unsigned int *swi_handler(unsigned int regs[16]) {
  (void)(regs);
  char syscall_number = *((char *)(regs[15] - 0x4));
  if (syscall_number == 0x1) {
    printf("\n");
    return thread_finish();
  } else {
    PANIC("syscall not implemented: %x\n", syscall_number);
  }
}

void __attribute__((interrupt ("ABORT"))) prefetch_abort_handler(void) {
  PANIC("PREFETCH_ABORT NOT IMPLEMENTED\n");
}

void __attribute__((interrupt ("ABORT"))) data_abort_handler(void) {
  int* abort_instruction;
  __asm volatile("mov %0, lr" : "=r" (abort_instruction));
  PANIC("data abort at %p\n", abort_instruction - 1);
}

unsigned int *irq_handler(unsigned int regs[16]) {
  (void)(AT91C_BASE_AIC->AIC_IVR);
  unsigned int system_time_status = AT91C_BASE_ST->ST_SR;
  unsigned int *return_regs = regs;
  if (system_time_status) {
    printf("!");
    return_regs = scheduler_tick(regs);
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
