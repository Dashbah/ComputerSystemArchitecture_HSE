#include <stdio.h>

// function: a + b * x ^ (-4)
// task: calculate integral from left to right
// using Trapezoidal rule

double left = -10, right = -4, a, b;
extern double getResult();
extern double checker();

int main() {
    // printf("a, b, left, right\n");
    int mistakes = 0;
    int tests = 0;
    for (double i = -10; i <= 10; i+=1) {
        for (double j = -10; j <= 10; j+=1) {
            a = i;
            b = j;
            ++tests;
            double myRes, expectedRes;
            myRes = getResult();
            expectedRes = checker();
            // printf("%f", myRes);
            // printf(" ");
            // printf("%f", expectedRes);
            if (myRes - expectedRes < 0.0001 && myRes - expectedRes > -0.0001) {
                // printf("\ntrue");
            } else {
                mistakes++;
            }
            // printf("\n");
        }
    }
    printf("num of mistakes: ");
    printf("%d", mistakes);
    printf("\nnum of tests: ");
    printf("%d", tests);
    printf("\n");
    return 0;
}
