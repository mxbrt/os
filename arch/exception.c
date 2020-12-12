#include <stdint.h>

#include <exception.h>
#include <dbgu.h>
#include <stdio.h>
#include <panic.h>

#include "at91rm9200.h"

void __attribute__((interrupt ("UNDEF"))) undef_handler(void) {
  int* undef_instruction;
  __asm volatile("mov %0, lr" : "=r" (undef_instruction));
  PANIC("PANIC: Undefined exception at %p\n", undef_instruction - 1);
}

void __attribute__((interrupt ("SWI"))) swi_handler(void) {
  int syscall_number;
  __asm volatile("ldrb %0, [lr, #-0x4]" : "=r" (syscall_number));
  printf("Got syscall: %x\n", syscall_number);
}

void __attribute__((interrupt ("ABORT"))) prefetch_abort_handler(void) {
  PANIC("PREFETCH_ABORT NOT IMPLEMENTED\n");
}


void __attribute__((interrupt ("ABORT"))) data_abort_handler(void) {
  int* abort_instruction;
  __asm volatile("mov %0, lr" : "=r" (abort_instruction));
  PANIC("data abort at %p\n", abort_instruction - 1);
}

void __attribute__((interrupt ("IRQ"))) irq_handler(void) {
  (void)(AT91C_BASE_AIC->AIC_IVR);
  volatile unsigned int system_time_status = AT91C_BASE_ST->ST_SR;
  if (system_time_status) {
    printf("!\n");
    goto out;
  }

  volatile unsigned int dbgu_status = AT91C_BASE_DBGU->DBGU_CSR;
  if (dbgu_status & AT91C_US_RXRDY) {
    dbgu_rx();
    goto out;
  }

  PANIC("unknown interrupt");

out:
  AT91C_BASE_AIC->AIC_EOICR = 1;
}

void __attribute__((interrupt ("FIQ"))) fiq_handler(void) {
  PANIC("FIQ NOT IMPLEMENTED\n");
}

