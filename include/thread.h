#ifndef __THREAD_H_
#define __THREAD_H_

#include <list.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>

#define N_THREADS 16
#define CPU_SLICE 1 * MSEC
#define THREAD_STACK_SIZE 4096

typedef enum {
    THREAD_FINISH,
    THREAD_BLOCKED,
    THREAD_RUNNING,
    THREAD_WAITING,
    THREAD_START,
} thread_state_t;

typedef struct tcb_t {
    unsigned int regs[16];
    unsigned int SPSR;
    uint8_t stack[THREAD_STACK_SIZE];
    va_list argv;
    thread_state_t state;
} tcb_t;

int thread_init(void *fn);
unsigned int* thread_finish(void);

void scheduler_init(void);
unsigned int* scheduler_tick(unsigned int regs[16]);

void thread_echo(void);
#endif
