/*
 * queue.h - FIFO queue of int values.
 *
 * DO NOT CHANGE THIS FILE. The automatic tests are written against it.
 *
 * Define `struct Queue` yourself inside src/queue.c.
 * Recommended: build the queue on top of your list (list.h).
 *
 * Rule: no loops (for / while / do-while), no goto and no recursion
 * anywhere in queue.c.
 */
#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>

typedef struct Queue Queue;

/* 1. Create a new, empty queue. Returns NULL if memory allocation fails. */
Queue *queue_create(void);

/* 2. Free the queue and everything in it. queue_destroy(NULL) does nothing. */
void queue_destroy(Queue *queue);

/* 3. Add a value to the END of the queue.
 *    Returns 0 on success, -1 if memory allocation fails. */
int queue_enqueue(Queue *queue, int value);

/* 4. Remove the FIRST value and store it in *out.
 *    Returns 0 on success, -1 if the queue is empty (*out is left unchanged). */
int queue_dequeue(Queue *queue, int *out);

/* 5. Number of values in the queue. */
size_t queue_length(const Queue *queue);

/* 6. Store the FIRST value in *out without removing it.
 *    Returns 0 on success, -1 if the queue is empty (*out is left unchanged). */
int queue_peek(const Queue *queue, int *out);

#endif /* QUEUE_H */
