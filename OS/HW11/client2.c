#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server_ip> <server_port>\n", argv[0]);
        return 1;
    }

    char *server_ip = argv[1];
    int server_port = atoi(argv[2]);

    // Создание сокета
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Failed to create socket");
        return 1;
    }

    // Настройка адреса сервера
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(server_ip);
    server_address.sin_port = htons(server_port);

    // Подключение к серверу
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Failed to connect to server");
        return 1;
    }

    printf("Connected to server %s:%d\n", server_ip, server_port);

    // Прием сообщений от сервера
    char buffer[MAX_BUFFER_SIZE];
    while (1) {
        // Чтение сообщения от сервера
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            break;
        }

        // Проверка на завершение
        if (strcmp(buffer, "The End\n") == 0) {
            break;
        }

        // Вывод принятого сообщения
        printf("Received message: %s", buffer);
    }

    // Закрытие сокета
    close(client_socket);

    printf("Client 2 shut down\n");

    return 0;
}