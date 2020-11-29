#include <stdint.h>

#include "exception.h"
#include "stdio.h"

void __attribute__((interrupt ("UNDEF"))) undef_handler(void) {
  int* undef_instruction;
  __asm volatile("mov %0, lr" : "=r" (undef_instruction));
  printf("PANIC: Undefined exception at %p\n", undef_instruction - 1);
  while(1) {}
}

void __attribute__((interrupt ("SWI"))) swi_handler(void) {
  int syscall_number;
  __asm volatile("ldrb %0, [lr, #-0x4]" : "=r" (syscall_number));
  printf("Got syscall: %x\n", syscall_number);
}

void __attribute__((interrupt ("ABORT"))) prefetch_abort_handler(void) {
  printf("PANIC: PREFETCH_ABORT NOT IMPLEMENTED\n");
  while(1) {}
}


void __attribute__((interrupt ("ABORT"))) data_abort_handler(void) {
  int* abort_instruction;
  __asm volatile("mov %0, lr" : "=r" (abort_instruction));
  printf("PANIC: data abort at %p\n", abort_instruction - 1);
  while(1) {}
}

void __attribute__((interrupt ("IRQ"))) irq_handler(void) {
  printf("PANIC: IRQ NOT IMPLEMENTED\n");
  while(1) {}
}

void __attribute__((interrupt ("FIQ"))) fiq_handler(void) {
  printf("PANIC: FIQ NOT IMPLEMENTED\n");
  while(1) {}
}

