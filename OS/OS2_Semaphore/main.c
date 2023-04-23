#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>

#define NUM_DEPARTMENTS 3
#define NUM_CUSTOMERS 5
#define SHARED_MEMORY_SIZE (sizeof(sem_t) * NUM_DEPARTMENTS)

void salesperson(int id, sem_t **semaphores, sem_t *shared_memory) {
    // int i, department;
    printf("Salesperson %d starts working.\n", id);
}

void customer(int id, sem_t **semaphores, sem_t *shared_memory) {
    int i;
    printf("Customer %d enters the store.\n", id);

    // choose random department
    int department = rand() % NUM_DEPARTMENTS + 1;
    // printf(department);

    // wait for salesperson to be available
    // int value;
    // sem_getvalue(semaphores[department], value);
    // printf(value);
    // if (value > 0) {
    //  }
    sem_wait(semaphores[department]);

    // release salesperson
    sem_post(shared_memory + department);
    // purchase item from department
    printf("Customer %d purchases item from department %d.\n", id, department);


    while (1) {
        // wait for customer
        for (i = 0; i < NUM_DEPARTMENTS; i++) {
            sem_wait(shared_memory + i);
        }

        // serve customer
        for (i = 0; i < NUM_DEPARTMENTS; i++) {
            if (sem_trywait(semaphores[i]) == 0) {
                printf("Salesperson %d serves customer in department %d.\n", id, i);

                // purchase item from department
                printf("Customer %d purchases item from department %d.\n", id, i);

                break;
            }
        }

        // release department semaphore
        sem_post(semaphores[i]);
    }
}

int main(void) {
    int i;
    pid_t pid;
    sem_t *semaphores[NUM_DEPARTMENTS];
    sem_t *shared_memory;
    void *ptr;

    // create shared memory for semaphores
    int shared_memory_fd = shm_open("/shared_memory", O_CREAT | O_RDWR, 0666);
    ftruncate(shared_memory_fd, SHARED_MEMORY_SIZE);
    ptr = mmap(0, SHARED_MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory_fd, 0);
    shared_memory = (sem_t *) ptr;

    // initialize semaphores
    for (i = 0; i < NUM_DEPARTMENTS; i++) {
        semaphores[i] = sem_open("/semaphore", O_CREAT, 0666, 1);
    }

    // create salespeople processes
    for (i = 0; i < NUM_DEPARTMENTS; i++) {
        pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(1);
        } else if (pid == 0) {
            salesperson(i, semaphores, shared_memory);
            exit(0);
        }
    }

    // create customer processes
    for (i = 0; i < NUM_CUSTOMERS; i++) {
        pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(1);
        } else if (pid == 0) {
            customer(i, semaphores, shared_memory);
            exit(0);
        }
    }

    // wait for child processes to finish
    for (i = 0; i < NUM_DEPARTMENTS + NUM_CUSTOMERS; i++) {
        wait(NULL);
        printf("wait for child processes to finish");
    }

    // destroy semaphores
    for (i = 0; i < NUM_DEPARTMENTS; i++) {
        sem_close(semaphores[i]);
        sem_unlink("/semaphore");
    }

    // unmap shared memory
    munmap(ptr, SHARED_MEMORY_SIZE);
    close(shared_memory_fd);
    shm_unlink("/shared_memory");

    return 0;
}

