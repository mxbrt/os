#include <dbgu.h>
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

void thread_tcb_init(tcb_t* tcb, void *fn) {
  memset(tcb, 0, sizeof(*tcb));
  tcb->state = THREAD_START;
  tcb->regs[13] = (size_t)&tcb->stack[THREAD_STACK_SIZE - sizeof(size_t)];
  tcb->regs[14] = (size_t)&syscall_exit;
  tcb->regs[15] = (size_t)fn + 0x4;
  tcb->SPSR = 0x1F |               // Set system mode
              (1 << 6);            // disable fiq
}

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

  thread_tcb_init(tcb, fn);

  if (thread_list_head == NULL) {
    thread_list_head = thread_node;
  } else {
    list_insert(thread_list_head, thread_node);
  }
  return 0;
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
    return thread_load(&idle_thread);
  } else {
    thread_list_head = thread_list_head->next;
    list_remove(thread_list_head->prev);
    return thread_load(thread_list_head->data);
  }
}

// Scheduler
void scheduler_init(void) {
  memset(thread_list, 0, sizeof(thread_list));
  memset(threads, 0, sizeof(threads));
  thread_list_head = NULL;
  thread_tcb_init(&idle_thread, &idle_thread_fn);
}

unsigned int* scheduler_tick(unsigned int regs[16]) {
  if (thread_list_head == NULL) {
    return thread_load(&idle_thread);
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
  return thread_load(thread_list_head->data);
}

// Threads
void thread_echo(void) {
  int c = getchar();
  for (int i = 0; i < 40; i++) {
    printf("%c", c);
    for (int j = 0; j < 3000000; j++) {};
  }
}

