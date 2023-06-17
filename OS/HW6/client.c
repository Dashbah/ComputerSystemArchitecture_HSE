#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>

#define SHM_KEY 12345

typedef struct {
    int number;
    int done;
} SharedData;

int main() {
    // Создание/получение идентификатора разделяемой памяти
    int shmid = shmget(SHM_KEY, sizeof(SharedData), 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("Failed to create/get shared memory segment");
        return 1;
    }

    // Получение указателя на разделяемую память
    SharedData *sharedData = (SharedData *)shmat(shmid, NULL, 0);
    if (sharedData == (SharedData *)-1) {
        perror("Failed to attach shared memory segment");
        return 1;
    }

    // Инициализация генератора случайных чисел
    srand(time(NULL));

    // Генерация и запись случайных чисел
    sharedData->done = 0;
    while (!sharedData->done) {
        int number = rand() % 100 + 1;
        sharedData->number = number;
        printf("Generated number: %d\n", number);
        usleep(500000); // Задержка 0.5 секунды
    }

    // Отключение разделяемой памяти
    shmdt(sharedData);

    return 0;
}
