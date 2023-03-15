#include <stdio.h>
#include "Func.c"
#include <unistd.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <string.h>
#include "ReaderWriter.c"

// char string[1048576] = "1234frv45fv45f23";
//char result[1048576];

const int size = 1048576;

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <string.h>


// вернём -1, если индекс не найден, вернём > 0, если нашли индекс
//int find_index(const char *string, const char *substring, int start) {
//    for (int i = start; string[i] != '\0'; ++i) {
//
//        int flag = 1;
//
//        for (int j = 0; substring[j] != '\0'; ++j) {
//            if (substring[j] != string[j + i]) {
//                flag = 0;
//                break;
//            }
//        }
//
//        if (flag == 1) {
//            return i;
//        }
//    }
//    return -1;
//}

//void find_all_indexes(const char *string, const char *substring, int *array) {
//    int i = 0;
//    int index = find_index(string, substring, 0);
//    array[i++] = index;
//
//    while (index != -1) {
//        index = find_index(string, substring, index + 1);
//        array[i++] = index;
//    }
//}

size_t countBytes(const int *array) {
    size_t count = 0;
    for (int i = 0; array[i] != -1; ++i) {
        ++count;
    }

    return count;
}

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
    // char *substring = argv[3];

    int child_1 = fork();
    int child_2 = fork();

    if (child_1 < 0 || child_2 < 0) {
        printf("Can\'t fork children\n");
        exit(-1);
    }

    if (child_1 > 0 && child_2 > 0) { /* Parent process */
        int status;
        wait(&status);

    } else if (child_1 == 0 && child_2 > 0) /* Child 1 process */
    {
        int fd;
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

        if (read_bytes > 8000) {
            printf("File is too big!");
            exit(-1);
        }

        write(file_d[1], buffer, read_bytes);
    } else if (child_1 > 0 && child_2 == 0) /* Child 2 process */
    {
        int status;
        waitpid(child_1, &status, 0);

        char str_buf[size];
        size_t read_bytes = read(file_d[0], str_buf, size);

        // обработка
        char array[read_bytes];
        func(str_buf,array);

//        // конвертируем в char*
//        char result[size];
//        int count = 0;
//        char local[20];
//        for (int i = 0; i < countBytes(array); ++i) {
//            sprintf(local, "%d\n", array[i]);
//            for (int j = 0; local[j] != '\n'; ++j) {
//                result[count++] = local[j];
//            }
//            result[count++] = ' ';
//        }

//        if (count == 0) {
//            printf("Substring not found!");
//        }
        // передача
        write(file_d[1], array, size);
    } else { /* Child 3 process */
        int status;
        waitpid(child_2, &status, 0);

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