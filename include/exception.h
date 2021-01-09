#ifndef __EXCEPTION_H_
#define __EXCEPTION_H_


#define N_EXCEPTIONS 8
extern int ivt[N_EXCEPTIONS * 2];

void __attribute__((interrupt ("UNDEF"))) undef_handler(void);
void __attribute__((interrupt ("ABORT"))) prefetch_abort_handler(void);
void __attribute__((interrupt ("ABORT"))) data_abort_handler(void);
void __attribute__((interrupt ("FIQ"))) fiq_handler(void);

// These interrupts may cause a context switch. They get the registers of the
// current thread and return a pointer to the registers of the next thread.
unsigned int *swi_handler(unsigned int regs[16]);
unsigned int *irq_handler(unsigned int regs[16]);

#endif // __EXCEPTION_H_
