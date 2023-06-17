#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define SHARED_MEMORY_NAME "/my_shared_memory"
#define SHARED_MEMORY_SIZE sizeof(int)

int main() {
    // Открытие разделяемой памяти
    int fd = shm_open(SHARED_MEMORY_NAME, O_RDONLY, 0666);
    if (fd == -1) {
        perror("Failed to open shared memory");
        return 1;
    }

    // Отображение разделяемой памяти в адресное пространство процесса
    int* sharedData = (int*)mmap(NULL, SHARED_MEMORY_SIZE, PROT_READ, MAP_SHARED, fd, 0);
    if (sharedData == MAP_FAILED) {
        perror("Failed to map shared memory");
        return 1;
    }

    // Чтение и вывод случайных чисел
    while (1) {
        printf("Received number: %d\n", *sharedData);
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

    return 0;
}
