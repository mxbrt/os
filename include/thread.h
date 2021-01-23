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
} thread_state_t;

typedef enum {
    THREAD_BLOCKED_SLEEP,
    THREAD_BLOCKED_IO_READ,
    THREAD_BLOCKED_IO_WRITE,
} thread_blocked_reason_t;

typedef struct thread_blocked_data_t {
    thread_blocked_reason_t reason;
    void *data;
} thread_blocked_t;

typedef struct tcb_t {
    unsigned int regs[16];
    unsigned int SPSR;
    uint8_t stack[THREAD_STACK_SIZE];
    va_list argv;
    thread_blocked_t blocked;
    thread_state_t state;
} tcb_t;

int thread_init(void *fn);
unsigned int* thread_finish(void);
unsigned int* thread_sleep(void);

void scheduler_init(void);
void scheduler_save_regs(unsigned int regs[16]);
unsigned int* scheduler_tick(void);

void thread_echo(void);
#endif
