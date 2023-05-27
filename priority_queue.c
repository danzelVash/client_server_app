#include <stdlib.h>
#include <string.h>
#include "priority_queue.h"

void init(PriorityQueue *queue, int capacity) {
    queue->nodes = (PriorityQueueNode *)malloc(sizeof(PriorityQueueNode) * capacity);
    queue->capacity = capacity;
    queue->size = 0;
}

void clear(PriorityQueue *queue) {
    free(queue->nodes);
    queue->nodes = NULL;
    queue->capacity = 0;
    queue->size = 0;
}

void swap(PriorityQueueNode *a, PriorityQueueNode *b) {
    PriorityQueueNode temp = *a;
    *a = *b;
    *b = temp;
}

void enqueue(PriorityQueue *queue, const char *data, int priority) {
    if (queue->size >= queue->capacity) {
        // Расширение очереди при необходимости
        int newCapacity = queue->capacity * 2;
        queue->nodes = (PriorityQueueNode *)realloc(queue->nodes, sizeof(PriorityQueueNode) * newCapacity);
        queue->capacity = newCapacity;
    }

    int index = queue->size;
    strcpy(queue->nodes[index].data, data);
    queue->nodes[index].priority = priority;
    queue->size++;

    // Поддержка свойств приоритетной очереди
    while (index > 0 && queue->nodes[index].priority < queue->nodes[(index - 1) / 2].priority) {
        swap(&queue->nodes[index], &queue->nodes[(index - 1) / 2]);
        index = (index - 1) / 2;
    }
}

void dequeue(PriorityQueue *queue, char *data, int *priority) {
    if (queue->size <= 0) {
        // Очередь пуста
        return;
    }

    strcpy(data, queue->nodes[0].data);
    *priority = queue->nodes[0].priority;

    queue->size--;
    strcpy(queue->nodes[0].data, queue->nodes[queue->size].data);
    queue->nodes[0].priority = queue->nodes[queue->size].priority;

    int index = 0;
    int childIndex;
    while (1) {
        childIndex = 2 * index + 1;
        if (childIndex >= queue->size) {
            break;
        }
        if (childIndex + 1 < queue->size && queue->nodes[childIndex + 1].priority < queue->nodes[childIndex].priority) {
            childIndex++;
        }
        if (queue->nodes[childIndex].priority < queue->nodes[index].priority) {
            swap(&queue->nodes[childIndex], &queue->nodes[index]);
            index = childIndex;
        } else {
            break;
        }
    }
}

int is_empty(PriorityQueue *queue) {
    return queue->size == 0;
}
