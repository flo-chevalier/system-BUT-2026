/*
 p2e8.c - cat (libC) avec préfixe N: largeur min 4
 Usage: p2e8 FILE
*/
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s FILE\n", argv[0]);
        return 2;
    }
    FILE *f = fopen(argv[1], "r");
    if (!f) {
        perror("fopen");
        return 1;
    }

    char *line = NULL;
    size_t cap = 0;
    ssize_t len;
    unsigned int lineno = 1;
    while ((len = getline(&line, &cap, f)) != -1) {
        /* largeur min 4 */
        printf("%4d: %s", lineno++, line);
        /* getline laisse le '\n' si présent */
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
