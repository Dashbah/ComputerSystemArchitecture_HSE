#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);

    // Создание сокета
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Failed to create socket");
        return 1;
    }

    // Настройка адреса сервера
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);

    // Привязка сокета к адресу сервера
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Failed to bind socket");
        return 1;
    }

    // Прослушивание входящих соединений
    if (listen(server_socket, 1) == -1) {
        perror("Failed to listen");
        return 1;
    }

    printf("Server listening on port %d...\n", port);

    // Принятие соединения от клиента №1
    int client1_socket = accept(server_socket, NULL, NULL);
    if (client1_socket == -1) {
        perror("Failed to accept client 1 connection");
        return 1;
    }

    printf("Client 1 connected\n");

    // Принятие соединения от клиента №2
    int client2_socket = accept(server_socket, NULL, NULL);
    if (client2_socket == -1) {
        perror("Failed to accept client 2 connection");
        return 1;
    }

    printf("Client 2 connected\n");

    // Передача сообщений от клиента №1 клиенту №2
    char buffer[MAX_BUFFER_SIZE];
    while (1) {
        // Чтение сообщения от клиента №1
        int bytes_received = recv(client1_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            break;
        }

        // Проверка на завершение
        if (strcmp(buffer, "The End\n") == 0) {
            break;
        }

        // Пересылка сообщения клиенту №2
        send(client2_socket, buffer, bytes_received, 0);
    }

    // Закрытие сокетов
    close(client1_socket);
    close(client2_socket);
    close(server_socket);

    printf("Server shut down\n");

    return 0;
}
