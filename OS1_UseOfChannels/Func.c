//
// Created by Dashbah on 15.03.2023.
//
#include <string.h>

void func(const char *string, char *result ,int *num) {
    // char result[5000];
    int result_len = 0;
    int set_plus = 1;
    for (int i = 0; i < strlen(string); ++i) {
        if (string[i] >= '0' && string[i] <= '9') {
            result[result_len++] = string[i];
            set_plus = 1;
        } else {
            if (set_plus) {
                result[result_len++] = '+';
                set_plus = 0;
            }
        }
    }
    *num = result_len;
    // return result;
}