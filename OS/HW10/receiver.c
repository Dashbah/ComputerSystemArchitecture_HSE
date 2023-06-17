#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int transmitter_pid;
int received_number = 0;
int bit_count = 0;

void handleSignal(int signal) {
    // Обработчик сигналов
    if (signal == SIGUSR1) {
        received_number <<= 1;
        bit_count++;
    } else if (signal == SIGUSR2) {
        received_number <<= 1;
        received_number |= 1;
        bit_count++;
    }

    if (bit_count == 32) {
        // Прием завершен, вывод результата
        printf("Received number: %d\n", received_number);
        exit(0);
    }
}

int main() {
    printf("Receiver PID: %d\n", getpid());

    // Запрос PID передатчика
    printf("Enter transmitter PID: ");
    scanf("%d", &transmitter_pid);

    // Установка обработчика сигналов
    signal(SIGUSR1, handleSignal);
    signal(SIGUSR2, handleSignal);

    while (1) {
        // Ожидание сигналов
        sleep(1);
    }

    return 0;
}
