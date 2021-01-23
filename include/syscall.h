#ifndef __SYSCALL_H_
#define __SYSCALL_H_

void syscall_exit(void);

char syscall_dbgu_read(void);
void syscall_dbgu_write(char x);
void syscall_thread_create(void *fn, void *args);
void syscall_sleep(int msec);

#endif // __SYSCALL_H_
