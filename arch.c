#include "at91rm9200.h"
#include "exception.h"
#include "string.h"
#include "time.h"
#include "panic.h"
#include "arch.h"

#define SCLK_FREQUENCY 32768

int get_cpsr(void);
void set_cpsr(int);

typedef enum {
  CPSR_IRQ = 1 << 7
} CPSR;

void enable_interrupts(void) {
  int cpsr = get_cpsr();
  cpsr &= ~CPSR_IRQ;
  set_cpsr(cpsr);
}

void disable_interrupts(void) {
  int cpsr = get_cpsr();
  cpsr |= CPSR_IRQ;
  set_cpsr(cpsr);
}

void init_exceptions(void) {
  // memory remap
  AT91C_BASE_MC->MC_RCR = AT91C_MC_RCB;

  // initialize exceptions
  memcpy((void*)0x4, ivt, sizeof(ivt));
}

void init_system_timer(void) {
  // setup system_timer
  unsigned int clock_cycles =
    (unsigned int)((float)SYSTEM_TIMER * ((float) SCLK_FREQUENCY / SEC));
  if (clock_cycles < 1) {
    PANIC("System Timer resolution insufficient");
  }

  AT91C_BASE_ST->ST_PIMR = clock_cycles;
  AT91C_BASE_ST->ST_IER = AT91C_ST_PITS;

  // enable system interrupt
  AT91C_BASE_AIC->AIC_IECR = 2;
}

void init(void) {
  init_exceptions();
  init_system_timer();
  enable_interrupts();
}
