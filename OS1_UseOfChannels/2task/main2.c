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
    int fd;
    char name[] = "main2.fifo";

    if (mknod(name, S_IFIFO | 0666, 0) < 0) {
        printf("Can\'t create FIFO\n");
        exit(-1);
    }

    if (argc != 3) {
        printf("Incorrect number of arguments!");
        return 0;
    }

    char *input_file = argv[1];
    char *output_file = argv[2];

    int child_1 = fork();
    int child_2 = fork();

    if (child_1 < 0 || child_2 < 0) {
        printf("Can\'t fork children\n");
        exit(-1);
    }

    if (child_1 > 0 && child_2 > 0) { /* Parent process */
        wait(NULL);
        wait(NULL);
        wait(NULL);
    } else if (child_1 == 0 && child_2 > 0) /* Child 1 process */
    {
        char buffer[size];
        ssize_t read_bytes;

        if ((fd = open(input_file, O_RDONLY)) < 0) {
            printf("Can\'t open file\n");
            exit(-1);
        }

        read_bytes = read(fd, buffer, size);

        if (read_bytes == -1) {
            printf("Can\'t write this file\n");
            exit(-1);
        }

        // запись в main.fifo
        if ((fd = open(name, O_WRONLY)) < 0) {
            printf("Can\'t open FIFO for writting\n");
            exit(-1);
        }
        write(fd, buffer, read_bytes);

    } else if (child_1 > 0 && child_2 == 0) /* Child 2 process */
    {
        waitpid(child_1, NULL, 0);

        //
        char str_buf[size];

        if ((fd = open(name, O_RDONLY)) < 0) {
            printf("Can\'t open FIFO for reading\n");
            exit(-1);
        }

        size_t read_bytes = read(fd, str_buf, size);

        // обработка
        char array[read_bytes];
        int numOfBytes;
        func(str_buf, array, &numOfBytes);



        // передача
        if ((fd = open(name, O_WRONLY)) < 0) {
            printf("Can\'t open FIFO for writting\n");
            exit(-1);
        }

        write(fd, array, numOfBytes);

    } else { /* Child 3 process */
        waitpid(child_2, NULL, 0);

        char buf[size];

        if ((fd = open(name, O_RDONLY)) < 0) {
            printf("Can\'t open FIFO for reading\n");
            exit(-1);
        }
        size_t read_bytes = read(fd, buf, size);

        if ((fd = open(output_file, O_WRONLY | O_CREAT, 0666)) < 0) {
            printf("Can\'t open file\n");
            exit(-1);
        }

        write(fd, buf, read_bytes);
    }

    return 0;
}