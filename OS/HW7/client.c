#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#define SHARED_MEMORY_NAME "/my_shared_memory"
#define SHARED_MEMORY_SIZE sizeof(int)

volatile sig_atomic_t is_running = 1;

void handleSignal(int signal) {
    if (signal == SIGINT) {
        is_running = 0;
    }
}

int main() {
    signal(SIGINT, handleSignal);

    // Открытие/создание разделяемой памяти
    int fd = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("Failed to create/open shared memory");
        return 1;
    }

    // Установка размера разделяемой памяти
    if (ftruncate(fd, SHARED_MEMORY_SIZE) == -1) {
        perror("Failed to set shared memory size");
        return 1;
    }

    // Отображение разделяемой памяти в адресное пространство процесса
    int* sharedData = (int*)mmap(NULL, SHARED_MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (sharedData == MAP_FAILED) {
        perror("Failed to map shared memory");
        return 1;
    }

    // Инициализация генератора случайных чисел
    srand(time(NULL));

    // Генерация и запись случайных чисел
    while (is_running) {
        int number = rand() % 100 + 1;
        *sharedData = number;
        printf("Generated number: %d\n", number);
        usleep(500000); // Задержка 0.5 секунды
    }

    // Отключение разделяемой памяти
    if (munmap(sharedData, SHARED_MEMORY_SIZE) == -1) {
        perror("Failed to unmap shared memory");
        return 1;
    }

    // Закрытие разделяемой памяти
    if (close(fd) == -1) {
        perror("Failed to close shared memory");
        return 1;
    }

    // Удаление разделяемой памяти
    if (shm_unlink(SHARED_MEMORY_NAME) == -1) {
        perror("Failed to unlink shared memory");
        return 1;
    }

    return 0;
}
