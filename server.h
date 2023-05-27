#ifndef SERVER_H
#define SERVER_H

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

extern int client_count;
extern int priority_mode;

extern pthread_mutex_t mutex;
extern pthread_cond_t cond;

typedef struct {
    int sockfd;
    struct sockaddr_in address;
    int priority;
} Client;

extern Client clients[MAX_NUM_OF_ACTIVE_CLIENTS];
extern PriorityQueue priority_queue;

void send_message_to_client(int client_index, const char *message);
void broadcast_message(const char *message, int priority);
void *client_handler(void *arg);
void *priority_handler(void *arg);

#endif  // SERVER_H
