#ifndef __SYSCALL_H_
#define __SYSCALL_H_

void syscall_exit(void);
void syscall_sleep(unsigned int msec);

char syscall_dbgu_read(void);
void syscall_dbgu_write(char c);
int syscall_thread_start(void *fn, unsigned int arg);

#endif // __SYSCALL_H_
