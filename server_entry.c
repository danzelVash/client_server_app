#include "server.h"

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
