#include <fifo.h>
#include <errno.h>

int fifo_put(fifo_t * fifo, char c) {
    if (fifo->cnt >= FIFO_SIZE) {
        return 0;
    }
    fifo->cnt++;
    fifo->buf[fifo->tx] = c;
    fifo->tx = (fifo->tx + 1) % FIFO_SIZE;
    return 1;
}

int fifo_get(fifo_t *fifo, char *c_ptr) {
    if (fifo->cnt == 0) {
        return 0;
    }
    fifo->cnt--;
    *c_ptr = fifo->buf[fifo->rx];
    fifo->rx = (fifo->rx + 1) % FIFO_SIZE;
    return 1;
}
