#include <stdlib.h>
#include "list.h"
#include "queue.h"

struct Queue {
    int dummy; /* TODO: replace (hint: a List *) */
};

Queue *queue_create(void)
{
    /* TODO */
    return NULL;
}

void queue_destroy(Queue *queue)
{
    /* TODO */
    (void)queue;
}

int queue_enqueue(Queue *queue, int value)
{
    /* TODO */
    (void)queue;
    (void)value;
    return -1;
}

int queue_dequeue(Queue *queue, int *out)
{
    /* TODO */
    (void)queue;
    (void)out;
    return -1;
}

size_t queue_length(const Queue *queue)
{
    /* TODO */
    (void)queue;
    return 0;
}

int queue_peek(const Queue *queue, int *out)
{
    /* TODO */
    (void)queue;
    (void)out;
    return -1;
}
