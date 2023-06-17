#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int receiver_pid;

void handleSignal(int signal) {
    // Обработчик сигналов
    if (signal == SIGUSR1) {
        printf("0");
    } else if (signal == SIGUSR2) {
        printf("1");
    }
}

int main() {
    printf("Transmitter PID: %d\n", getpid());

    // Запрос PID приемника
    printf("Enter receiver PID: ");
    scanf("%d", &receiver_pid);

    // Установка обработчика сигналов
    signal(SIGUSR1, handleSignal);
    signal(SIGUSR2, handleSignal);

    // Запрос ввода числа
    int number;
    printf("Enter a decimal number: ");
    scanf("%d", &number);

    // Передача числа побитово
    for (int i = 31; i >= 0; i--) {
        int bit = (number >> i) & 1;
        if (bit == 0) {
            kill(receiver_pid, SIGUSR1);
        } else {
            kill(receiver_pid, SIGUSR2);
        }
        usleep(10000); // Задержка 0.01 секунды
    }

    printf("\n");

    return 0;
}
