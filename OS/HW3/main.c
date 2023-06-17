#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Функция для вычисления числа Фибоначчи
int fibonacci(int n) {
    if (n <= 1)
        return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

// Функция для вычисления факториала
int factorial(int n) {
    int result = 1;
    for (int i = 1; i <= n; i++)
        result *= i;
    return result;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <number>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    pid_t pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        return 1;
    }

    if (pid == 0) {
        // Дочерний процесс вычисляет факториал
        int result = factorial(n);
        printf("Factorial of %d is: %d\n", n, result);
    } else {
        // Родительский процесс вычисляет число Фибоначчи
        int result = fibonacci(n);
        printf("Fibonacci number at position %d is: %d\n", n, result);
    }

    return 0;
}
