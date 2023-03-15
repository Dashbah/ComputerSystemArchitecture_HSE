#include <stdio.h>
#include "Func.c"

char string[1048576] = "1234frv45fv45f23";
char result[1048576];

int main() {
    func(string, result);
    printf(result);
    return 0;
}