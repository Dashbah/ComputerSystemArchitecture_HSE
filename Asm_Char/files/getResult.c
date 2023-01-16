#include <stdbool.h>
//
// Created by Dashbah on 09.11.2022.
//

extern char str[];
extern int checker;

bool getResult() {
    for (int i = 0; str[i] != '\0'; ++i) {
        if (str[i] == '(') {
            ++checker;
            continue;
        }
        if (str[i] == ')') {
            --checker;
            if (checker < 0) {
                return false;
            }
            continue;
        }
    }
    if (checker > 0) {
        return false;
    }
    return true;
}