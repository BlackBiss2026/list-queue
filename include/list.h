/*
 * list.h - Doubly linked list of int values.
 *
 * DO NOT CHANGE THIS FILE. The automatic tests are written against it.
 *
 * The structs are opaque: define `struct List` and `struct ListNode`
 * yourself inside src/list.c. Code outside list.c only sees pointers.
 *
 * Rule: no loops (for / while / do-while), no goto and no recursion
 * anywhere in list.c and queue.c, except inside list_destroy().
 */
#ifndef LIST_H
#define LIST_H

#include <stddef.h>

typedef struct List List;
typedef struct ListNode ListNode;

/* 1. Create a new, empty list. Returns NULL if memory allocation fails. */
List *list_create(void);

/* 2. Free every node and the list itself. list_destroy(NULL) does nothing. */
void list_destroy(List *list);

/* 3. Add a value to the END of the list.
 *    Returns a pointer to the new node, or NULL if allocation fails. */
ListNode *list_add(List *list, int value);

/* 4. Remove `node` from `list` and free it.
 *    `node` is guaranteed to belong to `list`. */
void list_remove(List *list, ListNode *node);

/* 5. Number of elements in the list. */
size_t list_length(const List *list);

/* 6. First node, or NULL if the list is empty. */
ListNode *list_first(const List *list);

/* 7. Last node, or NULL if the list is empty. */
ListNode *list_last(const List *list);

/* 8. The node after `node`, or NULL if `node` is the last one. */
ListNode *list_next(const ListNode *node);

/* 9. The node before `node`, or NULL if `node` is the first one. */
ListNode *list_prev(const ListNode *node);

/* 10. The value stored in `node`. */
int list_value(const ListNode *node);

/* 11. Average of all values in the list. Returns 0.0 for an empty list. */
double list_average(const List *list);

#endif /* LIST_H */
