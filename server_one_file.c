#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "priority_queue.h"

#define MAX_NUM_OF_ACTIVE_CLIENTS 100
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int client_count = 0;
int priority_mode = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

typedef struct {
    int sockfd;
    struct sockaddr_in address;
    int priority;
} Client;

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


int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    pthread_t thread_id;
    int client_index;

    pthread_t priority_thread_id;

    init(&priority_queue, MAX_NUM_OF_ACTIVE_CLIENTS);

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);
    pthread_create(&priority_thread_id, NULL, priority_handler, NULL);

    // Bind
    if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server started. Waiting for incoming connections...\n");

    while (1) {
        socklen_t client_len = sizeof(client_addr);
        client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &client_len);

        pthread_mutex_lock(&mutex);

        client_index = client_count;
        clients[client_index].sockfd = client_fd;
        clients[client_index].address = client_addr;
        clients[client_index].priority = client_count;

        client_count++;
        if (client_count > MAX_CLIENTS) {
            priority_mode = 1;
        } else {
            priority_mode = 0;
        }

        pthread_create(&thread_id, NULL, client_handler, &client_index);

        pthread_mutex_unlock(&mutex);
    }

    clear(&priority_queue);
    return 0;
}
