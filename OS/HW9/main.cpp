#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define SEM_KEY 1234

void child_process(int pipefd[2], int semid) {
    int i, value;
    char buf[256];

    for (i = 0; i < 10; i++) {
        // Ожидаем разрешения на чтение
        struct sembuf sem_op = {0, -1, 0};
        semop(semid, &sem_op, 1);

        // Читаем сообщение от родительского процесса
        read(pipefd[0], buf, sizeof(buf));
        printf("Child received: %s\n", buf);

        // Увеличиваем счетчик для разрешения записи родительскому процессу
        sem_op.sem_op = 1;
        semop(semid, &sem_op, 1);

        // Отправляем ответное сообщение
        sprintf(buf, "Message %d from child", i);
        write(pipefd[1], buf, sizeof(buf));
        // Увеличиваем счетчик для разрешения записи родительскому процессу
        sem_op.sem_op = 1;
        semop(semid, &sem_op, 1);
    }

    // Закрываем каналы и удаляем семафор
    close(pipefd[0]);
    close(pipefd[1]);
    semctl(semid, 0, IPC_RMID);
}

void parent_process(int pipefd[2], int semid) {
    int i;
    char buf[256];

    for (i = 0; i < 35; i++) {
        // Отправляем сообщение дочернему процессу
        sprintf(buf, "Message %d from parent", i);
        write(pipefd[1], buf, sizeof(buf));

        // Увеличиваем счетчик для разрешения записи дочернему процессу
        struct sembuf sem_op = {0, 1, 0};
        semop(semid, &sem_op, 1);

        // Ожидаем разрешения на чтение
        sem_op.sem_op = -1;
        semop(semid, &sem_op, 1);

        // Читаем ответное сообщение от дочернего процесса
        read(pipefd[0], buf, sizeof(buf));
        printf("Parent received: %s\n", buf);

        // Увеличиваем счетчик для разрешения записи дочернему процессу
        sem_op.sem_op = 1;
        semop(semid, &sem_op, 1);
    }

    // Закрываем каналы и удаляем семафор
    close(pipefd[0]);
    close(pipefd[1]);
    semctl(semid, 0, IPC_RMID);
}

int main() {
    int pipefd[2];
    pid_t pid;
    int semid;

    // Создаем канал
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Создаем семафор
    semid = semget(SEM_KEY, 1, IPC_CREAT | IPC_EXCL | 0666);
    if (semid == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    // Инициализируем счетчик семафора
    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    } arg{};
    arg.val = 1;
    if (semctl(semid, 0, SETVAL, arg) == -1) {
        perror("semctl");
        exit(EXIT_FAILURE);
    }

    // Создаем дочерний процесс
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Дочерний процесс
        child_process(pipefd, semid);
        exit(EXIT_SUCCESS);
    } else {
        // Родительский процесс
        parent_process(pipefd, semid);

        // Ожидаем завершения дочернего процесса
        wait(nullptr);
        exit(EXIT_SUCCESS);
    }
}