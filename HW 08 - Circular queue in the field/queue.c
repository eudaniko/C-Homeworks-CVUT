#include "queue.h"
#include <string.h>

static bool resize_queue(queue_t *queue, int new_capacity);

/* Create a new queue with a given size */
queue_t* create_queue(int capacity) {
    queue_t *queue = (queue_t*)malloc(sizeof(queue_t));
    if (!queue) {
        return NULL;
    }
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;
    queue->array = (void**)malloc(queue->capacity * sizeof(void*));
    if (!queue->array) {
        free(queue);
        return NULL;
    }
    return queue;
}

/* Delete the queue and all allocated memory */
void delete_queue(queue_t *queue) {
    if (queue) {
        free(queue->array);
        free(queue);
    }
}

/* Check if the queue is empty */
static bool is_empty(queue_t *queue) {
    return (queue->size == 0);
}

/* Inserts a reference to the element into the queue */
bool push_to_queue(queue_t *queue, void *data) {
    if (queue->size == queue->capacity) {
        if (!resize_queue(queue, queue->capacity * 2)) {
            return false;
        }
    }
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = data;
    queue->size++;
    return true;
}

/* Gets the first element from the queue and removes it */
void* pop_from_queue(queue_t *queue) {
    if (is_empty(queue)) {
        return NULL;
    }
    void *item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size--;
    if (queue->capacity > 3 && queue->size < queue->capacity / 3) {
        resize_queue(queue, queue->capacity / 2);
    }
    return item;
}

/* Gets idx-th element from the queue */
void* get_from_queue(queue_t *queue, int idx) {
    if (idx < 0 || idx >= queue->size) {
        return NULL;
    }
    int actualIdx = (queue->front + idx) % queue->capacity;
    return queue->array[actualIdx];
}

/* Gets number of stored elements */
int get_queue_size(queue_t *queue) {
    return queue->size;
}

/* Resizes the queue to the new capacity */
static bool resize_queue(queue_t *queue, int new_capacity) {
    void **new_array = (void**)malloc(new_capacity * sizeof(void*));
    if (!new_array) {
        return false;
    }

    if (queue->size == 0) {
        free(queue->array);
        queue->array = new_array;
        queue->capacity = new_capacity;
        queue->front = 0;
        queue->rear = -1;
        return true;
    }

    int current = queue->front;
    for (int i = 0; i < queue->size; i++) {
        new_array[i] = queue->array[current];
        current = (current + 1) % queue->capacity;
    }

    free(queue->array);
    queue->array = new_array;
    queue->capacity = new_capacity;
    queue->front = 0;
    queue->rear = queue->size - 1;
    
    return true;
}

