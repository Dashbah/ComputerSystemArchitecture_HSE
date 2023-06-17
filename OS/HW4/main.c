#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    char *inputFile = argv[1];
    char *outputFile = argv[2];

    // Открытие входного файла для чтения
    int inputFd = open(inputFile, O_RDONLY);
    if (inputFd == -1) {
        perror("Failed to open input file");
        return 1;
    }

    // Создание выходного файла с правами доступа, соответствующими входному файлу
    struct stat fileStat;
    if (stat(inputFile, &fileStat) == -1) {
        perror("Failed to get input file information");
        close(inputFd);
        return 1;
    }

    mode_t outputMode = fileStat.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);
    int outputFd = open(outputFile, O_CREAT | O_WRONLY | O_TRUNC, outputMode);
    if (outputFd == -1) {
        perror("Failed to create output file");
        close(inputFd);
        return 1;
    }

    // Чтение и запись файлов
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead, bytesWritten;

    while ((bytesRead = read(inputFd, buffer, BUFFER_SIZE)) > 0) {
        bytesWritten = write(outputFd, buffer, bytesRead);
        if (bytesWritten == -1) {
            perror("Failed to write to output file");
            close(inputFd);
            close(outputFd);
            return 1;
        }
    }

    if (bytesRead == -1) {
        perror("Failed to read from input file");
        close(inputFd);
        close(outputFd);
        return 1;
    }

    // Закрытие файлов
    close(inputFd);
    close(outputFd);

    printf("File copied successfully.\n");
    return 0;
}
