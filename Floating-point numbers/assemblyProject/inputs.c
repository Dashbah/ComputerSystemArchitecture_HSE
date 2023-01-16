#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
//
// Created by Dashbah on 09.11.2022.
//

extern double left, right, a, b;

bool consoleInput() {
    printf("a, b, left, right: \n");
    scanf("%lf", &a);
    scanf("%lf", &b);
    scanf("%lf", &left);
    scanf("%lf", &right);
    if ((left <= 0 && right >= 0) || (left >= right)) {
        return false;
    }
    return true;
}

bool fileInput() {
    FILE *input;
    char fileName[50];
    printf("Enter input file name: ");
    scanf("%s", fileName);
    if (access(fileName, F_OK) == 0) {
        input = fopen(fileName, "r");
        fscanf(input, "%lf", &a);
        fscanf(input, "%lf", &b);
        fscanf(input, "%lf", &left);
        fscanf(input, "%lf", &right);
        return true;
    }
    return false;
}

void randomGeneration() {
    time_t t;
    srand((unsigned) time(&t));
    a = rand() % 20 - 10;
    b = rand() % 20 - 10;
    left = rand() % 5 + 0.01;
    right = left + rand() % 10;
    printf("Your parameters: \n");
    printf("a: ");
    printf("%lf", a);
    printf(" b: ");
    printf("%lf", b);
    printf(" left: ");
    printf("%lf", left);
    printf(" right: ");
    printf("%lf", right);
    printf("\n");
}