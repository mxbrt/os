#include <dbgu.h>
#include <list.h>
#include <panic.h>
#include <stdio.h>
#include <string.h>
#include <syscall.h>
#include <thread.h>

tcb_t threads[N_THREADS];
list_t thread_list[N_THREADS];
list_t *thread_list_head;

// Threads
unsigned int get_spsr(void);
void set_spsr(unsigned int);

int thread_init(void *fn) {
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
    return 1;
  }

  tcb_t *tcb = &threads[free_idx];
  list_t *thread_node = &thread_list[free_idx];
  list_init(thread_node, tcb);

  if (thread_list_head == NULL) {
    thread_list_head = thread_node;
  } else {
    list_insert(thread_list_head, thread_node);
  }

  memset(tcb, 0, sizeof(*tcb));
  tcb->state = THREAD_START;
  tcb->regs[13] = (size_t)&tcb->stack[THREAD_STACK_SIZE - sizeof(size_t)];
  tcb->regs[14] = (size_t)&syscall_exit;
  tcb->regs[15] = (size_t)fn + 0x4;
  tcb->SPSR = 0x1F |               // Set system mode
              (1 << 6);            // disable fiq
  return 0;
}

unsigned int* thread_load_head(void) {
  // set next thread context
  tcb_t *thread = thread_list_head->data;
  set_spsr(thread->SPSR);
  thread->state = THREAD_RUNNING;
  return thread->regs;
}

unsigned int* thread_finish(void) {
  if (thread_list_head == NULL) {
    PANIC("NO THREADS");
  }
  tcb_t *cur_thread = thread_list_head->data;
  memset(cur_thread, 0, sizeof(*cur_thread));
  cur_thread->state = THREAD_FINISH;

  thread_list_head = thread_list_head->next;
  list_remove(thread_list_head->prev);
  return thread_load_head();
}

// Scheduler
void scheduler_init(void) {
  memset(thread_list, 0, sizeof(thread_list));
  memset(threads, 0, sizeof(threads));
  thread_list_head = NULL;
}

unsigned int* scheduler_tick(unsigned int regs[16]) {
  if (thread_list_head == NULL) {
    PANIC("NO THREADS");
  }

  if (thread_list_head->next != thread_list_head) {
    printf("\n");
  }

  tcb_t *cur_thread = thread_list_head->data;
  thread_list_head = thread_list_head->next;

  // save current thread context
  if (cur_thread->state == THREAD_RUNNING) {
    cur_thread->SPSR = get_spsr();
    memcpy(cur_thread->regs, regs, 16*4);
  }
  cur_thread->state = THREAD_WAITING;
  return thread_load_head();
}


// Threads
void thread_echo(void) {
  int c = getchar();
  for (int i = 0; i < 5; i++) {
    printf("%c", c);
    for (int j = 0; j < 30000000; j++) {};
  }
}
