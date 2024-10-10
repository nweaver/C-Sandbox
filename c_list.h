#ifndef _C_LIST_H
#define _C_LIST_H

#include <stdbool.h>

typedef struct linked_list_cell {
    void * data;
    struct linked_list_cell *next;
} linked_list_cell;

typedef struct linked_list {
   struct linked_list_cell *head;
   size_t length;
} linked_list;

linked_list * new_linked_list();

void prepend(linked_list *l, void *data);
void append(linked_list *l, void *data);

// Returns NULL if out of range
void *get_at(linked_list *l, size_t index);

void set_at(linked_list *l, size_t index, void *data);

// Returns -1 if not found, otherwise returns index
int find_at(linked_list *l, bool (*matcher)(void *));

void free_linked_list(linked_list *l);

#endif