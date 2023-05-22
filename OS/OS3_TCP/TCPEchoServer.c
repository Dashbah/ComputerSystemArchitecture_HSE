#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define MAX_CUSTOMERS 100
#define MAX_DEPARTMENTS 3
#define BUFFER_SIZE 1024

struct ShopServer {
    int server_socket;
    struct sockaddr_in server_address;
    int shop_departments[MAX_DEPARTMENTS];
    pthread_t customer_threads[MAX_CUSTOMERS];
    int customer_count;
    pthread_mutex_t lock;
};

struct CustomerArgs {
    int client_socket;
    struct ShopServer *shop;
};

void* handle_customer(void *args) {
    struct CustomerArgs *customer_args = (struct CustomerArgs *)args;
    struct ShopServer *shop = customer_args->shop;
    int client_socket = customer_args->client_socket;
    char buffer[BUFFER_SIZE];

    while (1) {
        // Receive customer request
        ssize_t num_bytes = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (num_bytes <= 0) {
            break;
        }

        // Process customer request
        int department_index = rand() % MAX_DEPARTMENTS;
        int department = shop->shop_departments[department_index];
        sprintf(buffer, "Customer %d is in Department %d\n", client_socket, department);

        // Send response to the customer
        send(client_socket, buffer, strlen(buffer), 0);

        // Simulate customer shopping time
        int shopping_time = rand() % 5 + 1;
        sleep(shopping_time);
    }

    // Close the customer socket
    close(client_socket);

    // Remove customer from the list after shopping
    pthread_mutex_lock(&shop->lock);
    for (int i = 0; i < shop->customer_count; i++) {
        if (shop->customer_threads[i] == pthread_self()) {
            shop->customer_threads[i] = shop->customer_threads[shop->customer_count - 1];
            shop->customer_count--;
            break;
        }
    }
    pthread_mutex_unlock(&shop->lock);

    free(customer_args);
    return NULL;
}

void start(struct ShopServer *shop, int port) {
    // Setup server socket
    shop->server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (shop->server_socket == -1) {
        perror("Failed to create server socket");
        exit(EXIT_FAILURE);
    }

    shop->server_address.sin_family = AF_INET;
    shop->server_address.sin_addr.s_addr = INADDR_ANY;
    shop->server_address.sin_port = htons(port);

    if (bind(shop->server_socket, (struct sockaddr *)&shop->server_address, sizeof(shop->server_address)) == -1) {
        perror("Failed to bind server socket");
        exit(EXIT_FAILURE);
    }

    if (listen(shop->server_socket, 5) == -1) {
        perror("Failed to listen on server socket");
        exit(EXIT_FAILURE);
    }

    printf("Shop server started on port %d!\n", port);

    while (1) {
        struct sockaddr_in client_address;
        socklen_t client_address_length = sizeof(client_address);
        int client_socket = accept(shop->server_socket, (struct sockaddr *)&client_address, &client_address_length);
        if (client_socket == -1) {
            perror("Failed to accept client connection");
            continue;
        }

        // Start a new thread to handle the customer
        pthread_t thread_id;
        struct CustomerArgs *customer_args = malloc(sizeof(struct CustomerArgs));
        customer_args->client_socket = client_socket;
        customer_args->shop = shop;
        pthread_create(&thread_id, NULL, handle_customer, (void *)customer_args);

        pthread_mutex_lock(&shop->lock);
        shop->customer_threads[shop->customer_count++] = thread_id;
        pthread_mutex_unlock(&shop->lock);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);
    srand(time(NULL));

    struct ShopServer shop;
    memset(&shop, 0, sizeof(struct ShopServer));
    shop.shop_departments[0] = 1;
    shop.shop_departments[1] = 2;
    shop.shop_departments[2] = 3;
    shop.customer_count = 0;
    pthread_mutex_init(&shop.lock, NULL);

    start(&shop, port);

    pthread_mutex_destroy(&shop.lock);
    return 0;
}
