#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int ARRAY_A[1048576];
int ARRAY_B[1048576];
int size_B;

extern void createRandomArray(int n);

void createFromFile() {
    int n, num;
    FILE *input, *output;
    input = fopen("input.txt", "r");
    fscanf(input, "%d", &n);
    for (int i = 0; i < n; ++i) {
        fscanf(input, "%d", &num);
        ARRAY_A[i] = num;
    }
}

void getResult(int n) {
    int average = 0, i;
    for (i = 0; i < n; ++i) {
        average += ARRAY_A[i];
    }
    average = average / n;
    int sum = ARRAY_A[0];
    size_B = 1;
    for (i = 0; ARRAY_A[i] <= average && i < n; ++i) {
        ARRAY_B[i] = sum;
        sum += ARRAY_A[i + 1];
        ++size_B;
    }
    ARRAY_B[i] = sum;
}

int main( )
{
    time_t t;
    int n, i;
    FILE *input, *output;
    srand((unsigned) time(&t));

    char type; // choosing fill type
    printf("for file input type f, for random : r");
    printf("\n");
    scanf("%c", &type);

    if (type == 'f') { // reading n and array from file
        int num;
        input = fopen("input.txt", "r");
        fscanf(input, "%d", &n);
        for (int i = 0; i < n; ++i) {
            fscanf(input, "%d", &num);
            ARRAY_A[i] = num;
        }
    } else if (type == 'r') { // random
        n = 5 + rand() % 20;
        createRandomArray(n);
    } else { // exit if command is incorrect
        printf("incorrect command");
        return 1;
    }

    printf("%d", '\n');

    getResult(n); // count average and fill ARRAY_B

    output = fopen("output.txt", "w");
    for (i = 0; i < size_B; ++i) {
        fprintf(output, "%d", ARRAY_B[i]);
        fprintf(output, " ");
    }
    return 0;
}