/*
 p2e9.c - head -n (libC)
 Usage: p2e9 [-n N] FILE
*/
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    int n = 10;
    const char *path = NULL;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            n = (int)strtol(argv[++i], NULL, 10);
            if (n < 0)
                n = 0;
        } else if (!path) {
            path = argv[i];
        } else {
            fprintf(stderr, "Usage: %s [-n N] FILE\n", argv[0]);
            return 2;
        }
    }
    if (!path) {
        fprintf(stderr, "Usage: %s [-n N] FILE\n", argv[0]);
        return 2;
    }
    FILE *f = fopen(path, "r");
    if (!f) {
        perror("fopen");
        return 1;
    }
    char *line = NULL;
    size_t cap = 0;
    ssize_t len;
    int count = 0;
    while (count < n && (len = getline(&line, &cap, f)) != -1) {
        fwrite(line, 1, (size_t)len, stdout);
        ++count;
    }
    if (ferror(f)) {
        perror("getline");
        free(line);
        fclose(f);
        return 1;
    }
    free(line);
    fclose(f);
    return 0;
}
