#include <dbgu.h>
#include <cpu.h>
#include <list.h>
#include <panic.h>
#include <stdio.h>
#include <string.h>
#include <syscall.h>
#include <thread.h>

tcb_t idle_thread;
tcb_t threads[N_THREADS];
list_t thread_list[N_THREADS];
list_t *thread_list_head;

// Threads
unsigned int get_spsr(void);
void set_spsr(unsigned int);

void idle_thread_fn(void) {
  while (1) {
    // TODO: enable sleep mode
  }
}

void thread_tcb_init(tcb_t* tcb, void *fn, unsigned int arg) {
  memset(tcb, 0, sizeof(*tcb));
  tcb->state = THREAD_READY;
  tcb->regs[0] = (unsigned int)arg;
  tcb->regs[13] = (size_t)&tcb->stack[THREAD_STACK_SIZE - sizeof(size_t)];
  tcb->regs[14] = (size_t)&syscall_exit;
  tcb->regs[15] = (size_t)fn + 0x4;
  tcb->SPSR = 0x1F |               // Set system mode
              (1 << 6);            // disable fiq
}

tcb_t* thread_create(void) {
  // find free tcb slot
  int free_idx = 0;
  while (free_idx < N_THREADS) {
    if (threads[free_idx].state == THREAD_FINISH) {
      break;
    }
    free_idx++;
  }

  if (free_idx >= N_THREADS) {
    WARN("Could not start thread: no capacity\n");
    return NULL;
  }

  tcb_t *tcb = &threads[free_idx];
  list_t *thread_node = &thread_list[free_idx];
  list_init(thread_node, tcb);


  if (thread_list_head == NULL) {
    thread_list_head = thread_node;
  } else {
    list_insert(thread_list_head, thread_node);
  }
  return tcb;
}

void thread_start(void *fn, unsigned int arg) {
  tcb_t *tcb = thread_create();
  if (tcb == NULL) {
    return;
  }
  thread_tcb_init(tcb, fn, arg);
}

unsigned int* thread_load(tcb_t* tcb) {
  // set next thread context
  set_spsr(tcb->SPSR);
  tcb->state = THREAD_RUNNING;
  return tcb->regs;
}

unsigned int* thread_finish(void) {
  if (thread_list_head == NULL) {
    PANIC("NO THREADS");
  }

  tcb_t *cur_thread = thread_list_head->data;
  memset(cur_thread, 0, sizeof(*cur_thread));
  cur_thread->state = THREAD_FINISH;

  if (thread_list_head->next == thread_list_head) {
    thread_list_head = NULL;
    return scheduler_tick();
  } else {
    thread_list_head = thread_list_head->next;
    list_remove(thread_list_head->prev);
    return scheduler_tick();
  }
}

unsigned int *thread_block(thread_blocked_reason_t reason, void* data) {
  if (thread_list_head == NULL) {
    PANIC("THREAD_BLOCK WITHOUT RUNNING THREAD");
  }

  tcb_t *cur_tcb = thread_list_head->data;
  cur_tcb->blocked.reason = reason;
  cur_tcb->blocked.data = (void*)data;
  cur_tcb->state = THREAD_BLOCKED;
  return scheduler_tick();
}

unsigned int* thread_sleep(unsigned int msec) {
  if (thread_list_head == NULL) {
    PANIC("SLEEP SYSCALL WITHOUT RUNNING THREAD");
  }

  unsigned int current_time = time();
  unsigned int wakeup = (current_time + msec) & ((1 << 20) - 1);
  return thread_block(THREAD_BLOCKED_SLEEP, (void*)wakeup);
}

static int thread_prepare(tcb_t *tcb, unsigned int current_time) {
  if (tcb->state == THREAD_BLOCKED) {
    if (tcb->blocked.reason == THREAD_BLOCKED_SLEEP) {
      unsigned int wakeup = (size_t)tcb->blocked.data;
      if (current_time > wakeup) {
        tcb->state = THREAD_READY;
        return 1;
      }
    } else if (tcb->blocked.reason == THREAD_BLOCKED_IO_READ) {
      char c;
      if (((io_callback_t*)tcb->blocked.data)(&c)) {
        tcb->regs[0] = c;
        tcb->state = THREAD_READY;
        return 1;
      }
    } else if (tcb->blocked.reason == THREAD_BLOCKED_IO_WRITE) {
      char c = (char)tcb->regs[0];
      if (((io_callback_t*)tcb->blocked.data)(&c)) {
        tcb->state = THREAD_READY;
        return 1;
      }
    }
  }
  if (tcb->state == THREAD_READY || tcb->state == THREAD_RUNNING) {
    return 1;
  }
  return 0;
}

// Scheduler
void scheduler_init(void) {
  memset(thread_list, 0, sizeof(thread_list));
  memset(threads, 0, sizeof(threads));
  thread_list_head = NULL;
  thread_tcb_init(&idle_thread, &idle_thread_fn, 0);
}

void scheduler_save_regs(unsigned int regs[16]) {
  tcb_t *cur_tcb = &idle_thread;
  if (thread_list_head != NULL &&
      ((tcb_t*)thread_list_head->data)->state == THREAD_RUNNING) {
    cur_tcb = thread_list_head->data;
  }

  if (cur_tcb->state == THREAD_RUNNING) {
    cur_tcb->SPSR = get_spsr();
    memcpy(cur_tcb->regs, regs, 16*4);
  }
}

unsigned int* scheduler_tick(void) {
  if (thread_list_head == NULL) {
    return thread_load(&idle_thread);
  }

  unsigned int current_time = time();

  list_t *cur_thread = thread_list_head;
  list_t *next_thread = thread_list_head->next;

  while (!thread_prepare(next_thread->data, current_time)
        && next_thread != cur_thread) {
    next_thread = next_thread->next;
  }

  tcb_t* next_tcb = next_thread->data;
  if (next_tcb->state != THREAD_READY &&
      next_tcb->state != THREAD_RUNNING) {
    return thread_load(&idle_thread);
  }

  tcb_t* cur_tcb = cur_thread->data;
  thread_list_head = next_thread;
  if (cur_tcb->state == THREAD_RUNNING) {
    cur_tcb->state = THREAD_READY;
  }

  return thread_load(thread_list_head->data);
}
