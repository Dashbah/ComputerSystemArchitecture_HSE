#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

void purchase(const char *server_ip, int server_port) {
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Failed to create client socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(server_ip);
    server_address.sin_port = htons(server_port);

    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Failed to connect to server");
        exit(EXIT_FAILURE);
    }

    // Send a request to the server
    char *request = "Purchase";
    send(client_socket, request, strlen(request), 0);

    // Receive and print the server's response
    char buffer[BUFFER_SIZE];
    ssize_t num_bytes = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
    if (num_bytes == -1) {
        perror("Failed to receive response from server");
        exit(EXIT_FAILURE);
    }

    buffer[num_bytes] = '\0';
    printf("%s\n", buffer);

    // Close the connection
    close(client_socket);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <server_ip> <server_port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *server_ip = argv[1];
    int server_port = atoi(argv[2]);

    purchase(server_ip, server_port);
    return 0;
}
