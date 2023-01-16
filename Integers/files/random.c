#include <stdlib.h>

extern int ARRAY_A[];

void createRandomArray(int n) {
    for (int i = 0; i < n; ++i) {
        ARRAY_A[i] = rand() % 100;
    }
}