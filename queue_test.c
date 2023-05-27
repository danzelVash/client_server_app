#include <stdio.h>
#include <string.h>
#include "priority_queue.h"

void test_priority_queue() {
    // Initialize the priority queue
    PriorityQueue queue;
    init(&queue, 5);

    // Test enqueue and dequeue operations
    enqueue(&queue, "Message 1", 2);
    enqueue(&queue, "Message 2", 1);
    enqueue(&queue, "Message 3", 3);

    char data[1024];
    int priority;

    dequeue(&queue, data, &priority);
    printf("Dequeued: %s (Priority: %d)\n", data, priority); // Expected: Message 2 (Priority: 1)

    dequeue(&queue, data, &priority);
    printf("Dequeued: %s (Priority: %d)\n", data, priority); // Expected: Message 1 (Priority: 2)

    // Test is_empty function
    int empty = is_empty(&queue);
    printf("Is empty: %d\n", empty); // Expected: 0 (false)

    // Test clear function
    clear(&queue);
    empty = is_empty(&queue);
    printf("Is empty after clear: %d\n", empty); // Expected: 1 (true)
}

int main() {
    test_priority_queue();

    return 0;
}
