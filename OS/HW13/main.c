
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_DEPTH 100

int recursive_open(char* filename, int depth) {
    if (depth > MAX_DEPTH) {
        return -1;
    }

    int fd = open(filename, O_RDONLY);

    if (fd == -1) {
        printf("Failed to open %sn", filename);
        return depth;
    }

    char symlink_name[10];
    sprintf(symlink_name, "%c%c", 'a' + depth, 'a' + depth);

    if (symlink(filename, symlink_name) == -1) {
        printf("Failed to create symlink %sn", symlink_name);
        close(fd);
        return depth;
    }

    printf("Opened %s, created symlink %sn", filename, symlink_name);

    return recursive_open(symlink_name, depth + 1);
}

int main() {
    char filename[] = "a";
    int depth = recursive_open(filename, 0);

    if (depth == -1) {
        printf("Maximum recursion depth reachedn");
    } else {
        printf("Recursion depth is %dn", depth);
    }

    return 0;
}
