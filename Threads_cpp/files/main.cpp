#include <thread>

extern bool consoleInput();
extern void randomGeneration();
extern bool fileInput();
extern double getResult();
extern double checker();

double a, b, left, right;

int main() {
    printf("Choose input type: \n");
    printf("For console input enter 1 \n");
    printf("For file input enter 2 \n");
    printf("For random generation enter 3 \n");
    int inputType;
    scanf("%d", &inputType);
    switch (inputType) {
        case 1: {
            if (!consoleInput()){
                printf("Incorrect limits of integration!");
                return 0;
            }
            break;
        }
        case 2: {
            if (!fileInput()){
                printf("Incorrect file name!");
                return 0;
            }
            break;
        }
        case 3: {
            randomGeneration();
            break;
        }
        default: {
            printf("Incorrect input!");
            return 0;
        }
    }

    FILE *output;
    output = fopen("output.txt", "w");

    double result;
    clock_t t = clock();
    result = getResult();
    t = clock() - t;

    double expectedResult = checker();

    printf("Result: ");
    printf("%f", result);
    printf("\nExpected result: ");
    printf("%f", expectedResult);

    fprintf(output, "%s", "\nResult: ");
    fprintf(output, "%f", result);
    fprintf(output, "%s", "\nExpected result: ");
    fprintf(output, "%f", expectedResult);

    printf("\nExecution time: ");
    printf("%d", (int)t);
    printf("\n");
    return 0;
}
