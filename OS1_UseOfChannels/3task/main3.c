//
// Created by Dashbah on 19.03.2023.
//
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <string.h>
#include "../Func.c"

const int size = 5000;

int main(int argc, char **argv) {
    int child1[2];
    int child2[2];

    if (pipe(child1) < 0) {
        printf("Can\'t open pipe\n");
        exit(-1);
    }

    if (pipe(child2) < 0) {
        printf("Can\'t open pipe\n");
        exit(-1);
    }

    if (argc != 3) {
        printf("Incorrect number of arguments!");
        return 0;
    }

    char *input_file = argv[1];
    char *output_file = argv[2];

    int child_1 = fork();
    int child_2;

    if (child_1 < 0) {
        printf("Can\'t fork children\n");
        exit(-1);
    }

    if (child_1 == 0) { /* Child 1 process */
        int fd;
        char buffer[size];
        ssize_t read_bytes;

        if ((fd = open(input_file, O_RDONLY)) < 0) {
            printf("Can\'t open file\n");
            exit(-1);
        }

        read_bytes = read(fd, buffer, size);

        printf("child 1 read from file: %s\n", buffer);

        if (read_bytes == -1) {
            printf("Can\'t write this file\n");
            exit(-1);
        }

        if (close(child1[0]) < 0) {
            printf("parent: Can\'t close reading side of pipe\n");
            exit(-1);
        }

        write(child1[1], buffer, read_bytes);

        if (close(child1[1]) < 0) {
            printf("parent: Can\'t close writing side of pipe\n");
            exit(-1);
        }

        if (close(child2[1]) < 0) {
            printf("parent: Can\'t close child writing side of pipe\n");
            exit(-1);
        }

        char str_buf[size];
        read_bytes = read(child2[0], str_buf, size);

        printf("child 1 get from child 2: %s\n", str_buf);

        if ((fd = open(output_file, O_WRONLY | O_CREAT, 0666)) < 0) {
            printf("Can\'t open file\n");
            exit(-1);
        }

        write(fd, str_buf, read_bytes);
    } else {
        child_2 = fork();
        if (child_2 == 0) { /* Child 2 process */
            char str_buf[size];

            if (close(child1[1]) < 0) {
                printf("child: Can\'t close writing side of pipe\n");
                exit(-1);
            }
            size_t read_bytes = read(child1[0], str_buf, size);

            if (close(child2[0]) < 0) {
                printf("child: Can\'t close reading side of pipe\n");
                exit(-1);
            }

            printf("child 2 get from child 1: %s\n", str_buf);

            char array[read_bytes];
            int numOfBytes;

            func(str_buf, array, &numOfBytes);

            write(child2[1], array, numOfBytes);

            if (close(child2[1]) < 0) {
                printf("child: Can\'t close child writing side of pipe\n");
                exit(-1);
            }

            if (close(child1[0]) < 0) {
                printf("child: Can\'t close reading side of pipe\n");
                exit(-1);
            }
        } else {
            wait(NULL);
        }
    }
    return 0;
}