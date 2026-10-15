#include <stdlib.h>
#include "list.h"

/* TODO: define the structs. Hint: what does the list need to remember
 * so that length and average can be answered without a loop? */
struct ListNode {
    int value;
    /* TODO */
};

struct List {
    int dummy; /* TODO: replace */
};

List *list_create(void)
{
    /* TODO */
    return NULL;
}

void list_destroy(List *list)
{
    /* TODO: the ONLY place a loop is allowed */
    (void)list;
}

ListNode *list_add(List *list, int value)
{
    /* TODO */
    (void)list;
    (void)value;
    return NULL;
}

void list_remove(List *list, ListNode *node)
{
    /* TODO */
    (void)list;
    (void)node;
}

size_t list_length(const List *list)
{
    /* TODO */
    (void)list;
    return 0;
}

ListNode *list_first(const List *list)
{
    /* TODO */
    (void)list;
    return NULL;
}

ListNode *list_last(const List *list)
{
    /* TODO */
    (void)list;
    return NULL;
}

ListNode *list_next(const ListNode *node)
{
    /* TODO */
    (void)node;
    return NULL;
}

ListNode *list_prev(const ListNode *node)
{
    /* TODO */
    (void)node;
    return NULL;
}

int list_value(const ListNode *node)
{
    /* TODO */
    (void)node;
    return 0;
}

double list_average(const List *list)
{
    /* TODO */
    (void)list;
    return 0.0;
}
