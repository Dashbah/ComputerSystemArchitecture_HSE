#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
//
// Created by Dashbah on 09.11.2022.
//

extern char str[];

void consoleInput() {
    printf("Enter your string: \n");
    scanf("%s", &str);
}

bool fileInput() {
    FILE *input;
    char fileName[50];
    printf("Enter input file name: ");
    scanf("%s", fileName);
    if (access(fileName, F_OK) == 0) {
        input = fopen(fileName, "r");
        fscanf(input, "%s", &str);
        return true;
    } else {
        return false;
    }
};

void randomGeneration() {
    time_t t;
    srand((unsigned) time(&t));
    int n = rand() % 50 + 3;
    for (int i = 0; i < n; ++i){
        // '!' is 33 in ASCII
        // this formula was chosen for more
        // '(' and ')' appearing in the string
        str[i] = '!' + rand() % 24;
    }
    printf("Your string: \n");
    printf("%s", str);
    printf("\n");
}