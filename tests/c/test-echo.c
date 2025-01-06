#include <stdio.h>

int main() {
    char *line = 0;
    size_t size;

    getline(&line, &size, stdin);
    printf("%s", line);

    return 0;
}
