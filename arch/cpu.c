#include <cpu.h>
#include <dbgu.h>
#include <exception.h>
#include <panic.h>
#include <string.h>
#include <time.h>
#include "at91rm9200.h"

#define SCLK_RESOLUTION 32768
#define INTERNAL_RAM ((void *)0x00200000)

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

static void init_exceptions(void) {
  // initialize exceptions
  memcpy(INTERNAL_RAM, ivt, sizeof(ivt));
  // memory remap
  AT91C_BASE_MC->MC_RCR = AT91C_MC_RCB;
}

static void init_system_timer(void) {
  // setup system_timer
  unsigned int clock_cycles =
    (unsigned int)((float)SYSTEM_TIMER * ((float) SCLK_RESOLUTION / SEC));
  if (clock_cycles < 1) {
    PANIC("System Timer resolution insufficient");
  }

  AT91C_BASE_ST->ST_PIMR = clock_cycles;
  AT91C_BASE_ST->ST_IER = AT91C_ST_PITS;
  AT91C_BASE_ST->ST_RTMR = 0x20; // = 1 millisecond
}

static void init_aic(void) {
  // enable system interrupts
  AT91C_BASE_AIC->AIC_IECR = 2;
}

unsigned int time(void) {
  return AT91C_BASE_ST->ST_CRTR;
}

void init(void) {
  init_exceptions();
  init_aic();
  init_dbgu();
  init_system_timer();
}
