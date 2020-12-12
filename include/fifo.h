#ifndef __FIFO_H_
#define __FIFO_H_

#include <stddef.h>

#define FIFO_SIZE 256

typedef struct {
    char buf[FIFO_SIZE];
    size_t rx;
    size_t tx;
    size_t cnt;
} fifo_t;

int fifo_put(fifo_t * fifo, char x);
int fifo_get(fifo_t *fifo, char *x);

#endif
