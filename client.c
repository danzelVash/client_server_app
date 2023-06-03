#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include "client.h"


void receive_message(int sockfd) {
    char buffer[BUFFER_SIZE];
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        ssize_t recv_size = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
        if (recv_size <= 0) {
            printf("Connection closed by the server.\n");
            break;
        }
        printf("Received: %s", buffer);
    }
}

void send_message(int sockfd) {
    char message[BUFFER_SIZE];
    while (1) {
        memset(message, 0, sizeof(message));
        printf("Enter message: ");
        fgets(message, sizeof(message), stdin);
        if (strcmp(message, "exit\n") == 0) {
            break;
        }
        ssize_t sent_size = send(sockfd, message, strlen(message), 0);
        if (sent_size <= 0) {
            printf("Error sending message.\n");
            break;
        }
    }
}
