#include <list.h>

void list_insert(struct list_t *head, struct list_t *new) {
    new->next = head->next;
    head->next->prev = new;
    head->next = new;
    new->prev = head;
}

void list_remove(struct list_t *node) {
    node->next->prev = node->prev;
    node->prev->next = node->next;
}

void list_init(struct list_t *node, void *data) {
    node->prev = node;
    node->next = node;
    node->data = data;
}
