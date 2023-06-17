#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_KEY 12345

typedef struct {
    int number;
    int done;
} SharedData;

int main() {
    // Получение идентификатора разделяемой памяти
    int shmid = shmget(SHM_KEY, sizeof(SharedData), 0666);
    if (shmid == -1) {
        perror("Failed to get shared memory segment");
        return 1;
    }

    // Получение указателя на разделяемую память
    SharedData *sharedData = (SharedData *)shmat(shmid, NULL, 0);
    if (sharedData == (SharedData *)-1) {
        perror("Failed to attach shared memory segment");
        return 1;
    }

    // Чтение и вывод случайных чисел
    while (!sharedData->done) {
        printf("Received number: %d\n", sharedData->number);
        usleep(500000); // Задержка 0.5 секунды
    }

    // Отключение разделяемой памяти
    shmdt(sharedData);

    // Удаление сегмента разделяемой памяти
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
