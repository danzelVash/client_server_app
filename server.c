#include "server.h"

int client_count = 0;
int priority_mode = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

Client clients[MAX_NUM_OF_ACTIVE_CLIENTS];
PriorityQueue priority_queue;

void send_message_to_client(int client_index, const char *message) {
    if (write(clients[client_index].sockfd, message, strlen(message)) < 0) {
        perror("Error sending message");
    }
}

void broadcast_message(const char *message, int priority) {
    pthread_mutex_lock(&mutex);

    if (priority_mode == 1) {
        enqueue(&priority_queue, message, priority);
    } else {
        for (int i = 0; i < client_count; i++) {
            if (clients[i].sockfd != -1 && priority != i) {
                send_message_to_client(i, message);
            }
        }
    }

    pthread_mutex_unlock(&mutex);
    pthread_cond_broadcast(&cond);
}

void *client_handler(void *arg) {
    int client_index = *(int *) arg;
    char buffer[BUFFER_SIZE];
    int read_size;

    while ((read_size = read(clients[client_index].sockfd, buffer, BUFFER_SIZE)) > 0) {
        buffer[read_size] = '\0';
        broadcast_message(buffer, clients[client_index].priority);
        memset(buffer, 0, BUFFER_SIZE);
    }

    pthread_mutex_lock(&mutex);
    clients[client_index].sockfd = -1;
    client_count--;

    if (client_count <= MAX_CLIENTS) {
        priority_mode = 0;
    }

    pthread_mutex_unlock(&mutex);
    pthread_cond_broadcast(&cond);
    close(clients[client_index].sockfd);

    pthread_mutex_lock(&mutex);
    clients[client_index] = clients[client_index + 1];
    clients[client_index].priority = client_index;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

void *priority_handler(void *arg) {
    while (1) {
        if (priority_mode == 1) {
            printf("priority_handler is running\n");
            pthread_mutex_lock(&mutex);

            while (!is_empty(&priority_queue)) {
                char message[BUFFER_SIZE];
                int priority;
                dequeue(&priority_queue, message, &priority);

                for (int i = 0; i < client_count; i++) {
                    if (clients[i].sockfd != -1) {
                        send_message_to_client(i, message);
                    }
                }
            }

            pthread_cond_wait(&cond, &mutex);
            pthread_mutex_unlock(&mutex);
            sleep(5);
        } else {
            printf("priority_handler is sleeping\n");
            sleep(10);
        }
    }
}
