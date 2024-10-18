#include <stdio.h>
#include <stdlib.h>

#include "c_list.h"

linked_list *new_linked_list()
{
    linked_list *ret = (linked_list *)malloc(
        sizeof(linked_list));
    ret->length = 0;
    ret->head = NULL;
    return ret;
};

void free_linked_list(linked_list *l)
{
    linked_list_cell *at = l->head;
    linked_list_cell *tmp = at;
    while (at)
    {
        tmp = at->next;
        free(at);
        at = tmp;
    }
    free(l);
}

void prepend(linked_list *l, void *data)
{
    linked_list_cell *node = malloc(sizeof(linked_list_cell));
    node->next = l->head;
    l->head = node;
    node->data = data;
    l->length += 1;
}

void append(linked_list *l, void *data)
{
    linked_list_cell *tmp = l->head;
    l->length += 1;
    if (l->head == NULL)
    {
        l->head = malloc(sizeof(linked_list_cell));
        l->head->next = NULL;
        l->head->data = data;
        return;
    }
    while (tmp->next != NULL)
    {
        tmp = tmp->next;
    }
    tmp->next = malloc(sizeof(linked_list_cell));
    tmp->next->data = data;
    tmp->next->next = NULL;
    return;
}

// Returns NULL if out of range
void *get_at(linked_list *l, size_t index)
{
    linked_list_cell *node = l->head;
    while (node)
    {
        if (index == 0)
        {
            return node->data;
        }
        node = node->next;
        index--;
    }
    return NULL;
}

void set_at(linked_list *l, size_t index, void *data)
{
    linked_list_cell *at = l->head;
    size_t i;
    while (at != NULL)
    {
        if (index == i)
        {
            at->data = data;
            return;
        }
        i++;
        at = at->next;
    }
}

// Returns -1 if not found, otherwise returns index
int find_at(linked_list *l, bool (*matcher)(void *))
{
    linked_list_cell *at = l->head;
    int i = 0;
    while (at != NULL)
    {
        if (matcher(at->data))
        {
            return i;
        }
        i++;
        at = at->next;
    }
    return -1;
}
