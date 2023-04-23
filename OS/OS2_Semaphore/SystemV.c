//
// Created by Dashbah on 23.04.2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>

#define NUM_DEPARTMENTS 3
#define NUM_CUSTOMERS 10
#define NUM_PURCHASES 2

int semid;

void customer(int id) {
    struct sembuf wait_op, signal_op;
    for (int i = 0; i < NUM_PURCHASES; ++i) {
        srand(getpid());
        int department = rand() % NUM_DEPARTMENTS;
        department += i;
        department %= NUM_DEPARTMENTS;
        printf("Customer %d is waiting for department %d.\n", id, department);

        wait_op.sem_num = department;
        wait_op.sem_op = -1;
        wait_op.sem_flg = 0;

        semop(semid, &wait_op, 1);

        printf("Customer %d purchases item from department %d.\n", id, department);
        // wait(100);

        signal_op.sem_num = department;
        signal_op.sem_op = 1;
        signal_op.sem_flg = 0;

        semop(semid, &signal_op, 1);
    }
}

int main() {
    printf("\n");

    // create semaphores
    semid = semget(IPC_PRIVATE, NUM_DEPARTMENTS, IPC_CREAT | 0600);
    if (semid == -1) {
        perror("semget");
        exit(1);
    }

    for (int i = 0; i < NUM_DEPARTMENTS; i++) {
        if (semctl(semid, i, SETVAL, 1) == -1) {
            perror("semctl");
            exit(1);
        }
    }

    // create customer processes
    for (int i = 0; i < NUM_CUSTOMERS; ++i) {
        int pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(1);
        } else if (pid > 0) {
            customer(i);
            exit(0);
        }
    }

    // wait for child processes to finish
    for (int i = 0; i < NUM_DEPARTMENTS + NUM_CUSTOMERS; i++) {
        wait(NULL);
        printf("wait for child processes %d to finish\n", i);
    }

    // destroy semaphores
    if (semctl(semid, 0, IPC_RMID) == -1) {
        perror("semctl");
        exit(1);
    }

    return 0;
}