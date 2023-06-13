#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_BUFFER_SIZE 1024
#define SERVER_PORT 12345
#define MAX_CLIENTS 5

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *client_handler(void *arg) {
    int newsockfd = *(int *)arg;
    char buffer[MAX_BUFFER_SIZE];

    // Serve the client
    while (1) {
        // Receive request from the client
        memset(buffer, 0, MAX_BUFFER_SIZE);
        if (recv(newsockfd, buffer, MAX_BUFFER_SIZE, 0) == -1) {
            perror("recv failed");
            break;
        }

        if (strcmp(buffer, "Enter") == 0) {
            printf("Client entered the shop.\n");

            // Simulate serving the client in different departments
            printf("Serving the client in Department 1...\n");
            sleep(3);  // Simulating service time

            printf("Serving the client in Department 2...\n");
            sleep(2);  // Simulating service time

            printf("Serving the client in Department 3...\n");
            sleep(4);  // Simulating service time

            printf("Client finished shopping.\n");

            // Send response to the client
            strcpy(buffer, "Thank you for visiting!");
            if (send(newsockfd, buffer, strlen(buffer), 0) == -1) {
                perror("send failed");
                break;
            }
        } else if (strcmp(buffer, "Leave") == 0) {
            printf("Client left the shop.\n");

            // Send response to the client
            strcpy(buffer, "Goodbye!");
            if (send(newsockfd, buffer, strlen(buffer), 0) == -1) {
                perror("send failed");
                break;
            }

            break;
        } else {
            printf("Invalid command received from the client.\n");

            // Send response to the client
            strcpy(buffer, "Invalid command!");
            if (send(newsockfd, buffer, strlen(buffer), 0) == -1) {
                perror("send failed");
                break;
            }
        }
    }

    // Close the socket and free resources
    close(newsockfd);
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

int main() {
    int sockfd, newsockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    pthread_t threads[MAX_CLIENTS];
    int num_clients = 0;

    // Create a TCP socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set server details
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERVER_PORT);

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for client connections
    if (listen(sockfd, MAX_CLIENTS) == -1) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening for client connections.\n");

    while (1) {
        // Accept a new client connection
        if ((newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_len)) == -1) {
            perror("accept failed");
            continue;
        }

        pthread_mutex_lock(&mutex);

        // Create a new thread to handle the client
        if (pthread_create(&threads[num_clients], NULL, client_handler, &newsockfd) != 0) {
            perror("thread creation failed");
            continue;
        }

        num_clients++;

        if (num_clients >= MAX_CLIENTS) {
            printf("Maximum number of clients reached. Cannot accept more connections.\n");
            break;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < num_clients; i++) {
        pthread_join(threads[i], NULL);
    }

    // Close the sockets
    close(sockfd);

    return 0;
}
