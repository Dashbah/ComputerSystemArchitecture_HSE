//
// Created by Dashbah on 23.04.2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <time.h>

int NUM_DEPARTMENTS = 3;
int NUM_CUSTOMERS = 10;
int NUM_PURCHASES = 2;

void customer(int id, sem_t **semaphores) {
    for (int i = 0; i < NUM_PURCHASES; ++i) {
        srand(getpid());
        int department = rand() % NUM_DEPARTMENTS;
        department += i;
        department %= NUM_DEPARTMENTS;
        printf("Customer %d is waiting for department %d.\n", id, department);
        sem_wait(semaphores[department]);
        printf("Customer %d purchases item from department %d.\n", id, department);
        // wait(100);
        sem_post(semaphores[department]);
    }
}

int main() {
    printf("\n");
    sem_t *semaphores[NUM_DEPARTMENTS];

    for (int i = 0; i < NUM_DEPARTMENTS; i++) {
        semaphores[i] = sem_open("/semaphore", 1);
        sem_post(semaphores[i]);
    }

    // create customer processes
    for (int i = 0; i < NUM_CUSTOMERS; ++i) {
        int pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(1);
        } else if (pid > 0) {
            customer(i, semaphores);
            exit(0);
        }
    }

    // wait for child processes to finish
    for (int i = 0; i < NUM_DEPARTMENTS + NUM_CUSTOMERS; i++) {
        wait(NULL);
        printf("wait for child processes %d to finish\n", i);
    }

    // destroy semaphores
    for (int i = 0; i < NUM_DEPARTMENTS; i++) {
        sem_close(semaphores[i]);
        printf("semaphore %d closed\n", i);
        sem_unlink("/semaphore");
    }

    return 0;
}