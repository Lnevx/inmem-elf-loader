#include <stdio.h>

int main() {
    char *line = 0;
    size_t size;

    (void)getline(&line, &size, stdin);
    printf("%s", line);
    return 0;
}
