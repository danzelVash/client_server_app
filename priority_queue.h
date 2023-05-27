#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

typedef struct {
    char data[1024];
    int priority;
} PriorityQueueNode;

typedef struct {
    PriorityQueueNode *nodes;
    int capacity;
    int size;
} PriorityQueue;

void init(PriorityQueue *queue, int capacity);
void clear(PriorityQueue *queue);
void enqueue(PriorityQueue *queue, const char *data, int priority);
void dequeue(PriorityQueue *queue, char *data, int *priority);
int is_empty(PriorityQueue *queue);

#endif /* PRIORITY_QUEUE_H */
