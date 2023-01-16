#include <stdio.h>
#include <stdbool.h>
#include <time.h>

char str[1048576];
int checker = 0;

extern void consoleInput();
extern void randomGeneration();
extern bool fileInput();
extern bool getResult();

int main() {
    printf("Choose input type: \n");
    printf("For console input enter 1 \n");
    printf("For file input enter 2 \n");
    printf("For random generation enter 3 \n");
    int inputType;
    scanf("%d", &inputType);
    switch (inputType) {
        case 1: {
            consoleInput();
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

    bool result;
    clock_t t = clock();
    for (int i = 0; i < 20000; ++i) {
        result = getResult();
    }
    t = clock() - t;

    if (result) {
        printf("Congrats! It is correct");
        fprintf(output, "%s", "Congrats! It is correct");
    } else {
        printf("Oops! It is not correct...");
        fprintf(output, "%s", "\"Oops! It is not correct...\"");
    }

    printf("\nExecution time: ");
    printf("%d", (int)t);
    return 0;
}
