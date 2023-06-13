#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[MAX_BUFFER_SIZE];

    // Create a TCP socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set server details
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT);

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to the server.\n");

    // Send request to the server
    strcpy(buffer, "Enter");
    if (send(sockfd, buffer, strlen(buffer), 0) == -1) {
        perror("send failed");
        exit(EXIT_FAILURE);
    }

    // Receive response from the server
    memset(buffer, 0, MAX_BUFFER_SIZE);
    if (recv(sockfd, buffer, MAX_BUFFER_SIZE, 0) == -1) {
        perror("recv failed");
        exit(EXIT_FAILURE);
    }
    printf("Server response: %s\n", buffer);

    // Send request to the server
    strcpy(buffer, "Leave");
    if (send(sockfd, buffer, strlen(buffer), 0) == -1) {
        perror("send failed");
        exit(EXIT_FAILURE);
    }

    // Receive response from the server
    memset(buffer, 0, MAX_BUFFER_SIZE);
    if (recv(sockfd, buffer, MAX_BUFFER_SIZE, 0) == -1) {
        perror("recv failed");
        exit(EXIT_FAILURE);
    }
    printf("Server response: %s\n", buffer);

    // Close the socket
    close(sockfd);

    return 0;
}
