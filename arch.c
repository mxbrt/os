#include "at91rm9200.h"
#include "exception.h"
#include "string.h"
#include "time.h"
#include "panic.h"

#define SCLK_FREQUENCY 32768

void init(void) {
  // memory remap
  AT91C_BASE_MC->MC_RCR = AT91C_MC_RCB;

  // initialize exceptions
  memcpy((void*)0x4, ivt, sizeof(ivt));

  // setup system_timer
  unsigned int usec = 1 * SEC;
  unsigned int clock_cycles = usec * SCLK_FREQUENCY / SEC;
  if (clock_cycles < 1) {
    PANIC("System Timer resolution insufficient");
  }

  AT91C_BASE_ST->ST_PIMR = clock_cycles;
  AT91C_BASE_ST->ST_IER = AT91C_ST_PITS;
}
