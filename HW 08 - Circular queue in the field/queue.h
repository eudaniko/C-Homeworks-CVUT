#ifndef __QUEUE_H__
#define __QUEUE_H__
 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Queue structure which holds all necessary data */
typedef struct {
    void **array; 
    int capacity;
    int front;
    int rear;
    int size;
} queue_t;

/* creates a new queue with a given size */
queue_t* create_queue(int capacity);

/* deletes the queue and all allocated memory */
void delete_queue(queue_t *queue);

/* inserts a reference to the element into the queue */
bool push_to_queue(queue_t *queue, void *data);

/* gets the first element from the queue and removes it from the queue */
void* pop_from_queue(queue_t *queue);

/* gets idx-th element from the queue */
void* get_from_queue(queue_t *queue, int idx);

/* gets number of stored elements */
int get_queue_size(queue_t *queue);

#endif /* __QUEUE_H__ */
