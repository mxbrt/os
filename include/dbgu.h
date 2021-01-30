#ifndef __DBGU_H_
#define __DBGU_H_

void init_dbgu(void);
void dbgu_rx(void);
void dbgu_tx(void);
int dbgu_getchar(char *c);
int dbgu_putchar(char c);

int getchar(void);
int putchar(int c);

void kmsg(const char* type, const char *msg);

#endif
