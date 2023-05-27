#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERVER_HOST "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

void receive_message(int sockfd);
void send_message(int sockfd);

#endif  // CLIENT_H
