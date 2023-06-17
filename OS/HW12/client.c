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
    int client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket == -1) {
        perror("Failed to create socket");
        return 1;
    }

    // Настройка адреса клиента
    struct sockaddr_in client_address;
    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = INADDR_ANY;
    client_address.sin_port = htons(SERVER_PORT);

    // Привязка сокета к адресу клиента
    if (bind(client_socket, (struct sockaddr *)&client_address, sizeof(client_address)) == -1) {
        perror("Failed to bind socket");
        return 1;
    }

    // Включение опции широковещательной рассылки
    int broadcast_enable = 1;
    if (setsockopt(client_socket, SOL_SOCKET, SO_BROADCAST, &broadcast_enable, sizeof(broadcast_enable)) == -1) {
        perror("Failed to enable broadcast");
        return 1;
    }

    printf("Client started\n");

    // Бесконечный цикл для приема сообщений от сервера
    while (1) {
        char buffer[MAX_BUFFER_SIZE];

        // Прием сообщения от сервера
        struct sockaddr_in server_address;
        socklen_t server_address_length = sizeof(server_address);
        int bytes_received = recvfrom(client_socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&server_address, &server_address_length);
        if (bytes_received == -1) {
            perror("Failed to receive message");
            break;
        }

        // Вывод полученного сообщения
        printf("Received message from server: %s", buffer);
    }

    // Закрытие сокета
    close(client_socket);

    printf("Client shut down\n");

    return 0;
}
