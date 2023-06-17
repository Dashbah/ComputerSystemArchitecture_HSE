#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 1024
#define SERVER_PORT 5000
#define BROADCAST_ADDRESS "255.255.255.255"

int main() {
    // Создание сокета
    int server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket == -1) {
        perror("Failed to create socket");
        return 1;
    }

    // Настройка адреса сервера
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(SERVER_PORT);

    // Привязка сокета к адресу сервера
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Failed to bind socket");
        return 1;
    }

    printf("Server started\n");

    // Бесконечный цикл для приема сообщений и их рассылки
    while (1) {
        char buffer[MAX_BUFFER_SIZE];

        // Прием сообщения от клиента
        struct sockaddr_in client_address;
        socklen_t client_address_length = sizeof(client_address);
        int bytes_received = recvfrom(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_address, &client_address_length);
        if (bytes_received == -1) {
            perror("Failed to receive message");
            break;
        }

        // Вывод полученного сообщения
        printf("Received message from client: %s", buffer);

        // Рассылка сообщения всем клиентам в локальной сети
        struct sockaddr_in broadcast_address;
        broadcast_address.sin_family = AF_INET;
        broadcast_address.sin_addr.s_addr = inet_addr(BROADCAST_ADDRESS);
        broadcast_address.sin_port = htons(SERVER_PORT);

        if (sendto(server_socket, buffer, bytes_received, 0, (struct sockaddr *)&broadcast_address, sizeof(broadcast_address)) == -1) {
            perror("Failed to broadcast message");
            break;
        }
    }

    // Закрытие сокета
    close(server_socket);

    printf("Server shut down\n");

    return 0;
}
