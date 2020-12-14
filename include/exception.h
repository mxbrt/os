#ifndef __EXCEPTION_H_
#define __EXCEPTION_H_


#define N_EXCEPTIONS 8
extern int ivt[N_EXCEPTIONS * 2];

void __attribute__((interrupt ("UNDEF"))) undef_handler(void);
void __attribute__((interrupt ("SWI"))) swi_handler(void);
void __attribute__((interrupt ("ABORT"))) prefetch_abort_handler(void);
void __attribute__((interrupt ("ABORT"))) data_abort_handler(void);
void __attribute__((interrupt ("IRQ"))) irq_handler(void);
void __attribute__((interrupt ("FIQ"))) fiq_handler(void);

#endif // __EXCEPTION_H_
