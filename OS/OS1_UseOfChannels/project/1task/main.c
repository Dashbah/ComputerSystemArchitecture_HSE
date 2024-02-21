#include <stdio.h>
#include "../Func.c"
#include <unistd.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <string.h>

const int size = 5000;

int main(int argc, char **argv) {
    int file_d[2];

    if (pipe(file_d) < 0) {
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
    int child_2 = fork();

    if (child_1 < 0 || child_2 < 0) {
        printf("Can\'t fork children\n");
        exit(-1);
    }

    if (child_1 > 0 && child_2 > 0) { /* Parent process */
        wait(NULL);
        wait(NULL);
        wait(NULL);
    } else if (child_1 == 0 && child_2 > 0) { /* Child 1 process */
        int fd;
        char buffer[size];
        ssize_t read_bytes;

        if ((fd = open(input_file, O_RDONLY)) < 0) {
            printf("Can\'t open file\n");
            exit(-1);
        }

        read_bytes = read(fd, buffer, size);

        if (read_bytes == -1) {
            printf("Can\'t read this file\n");
            exit(-1);
        }

        write(file_d[1], buffer, read_bytes);

    } else if (child_1 > 0 && child_2 == 0) { /* Child 2 process */
        waitpid(child_1, NULL, 0);

        char str_buf[size];
        size_t read_bytes = read(file_d[0], str_buf, size);
        
        char array[read_bytes];
        int numOfBytes;
        func(str_buf,array, &numOfBytes);

        write(file_d[1], array, numOfBytes);
    } else { /* Child 3 process */
        waitpid(child_2, NULL, 0);

        char buf[size];

        size_t read_bytes = read(file_d[0], buf, size);

        int fd;

        if ((fd = open(output_file, O_WRONLY | O_CREAT, 0666)) < 0) {
            printf("Can\'t open file\n");
            exit(-1);
        }

        write(fd, buf, read_bytes);
    }
    return 0;
}
