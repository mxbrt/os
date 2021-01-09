#ifndef __LIST_H_
#define __LIST_H_

#include <stddef.h>

typedef struct list_t {
    struct list_t *next;
    struct list_t *prev;
    void *data;
} list_t;

void list_insert(struct list_t *next, struct list_t *new);
void list_remove(struct list_t *node);
void list_init(struct list_t *node, void *data);

#endif
