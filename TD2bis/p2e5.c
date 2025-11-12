/*
 p2e5.c - I/O binaire (libc)
 Écrit un tableau d'int32 dans p2e5.dat via fwrite puis le relit via fread.
*/
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int main(void) {
    int32_t arr[] = {INT32_MIN,
                     (int32_t)(INT32_MIN + 1),
                     -2048,
                     -1,
                     0,
                     1,
                     2048,
                     (int32_t)(INT32_MAX - 1),
                     INT32_MAX};

    size_t n = sizeof(arr) / sizeof(arr[0]);

    FILE *f = fopen("p2e5.dat", "wb");
    if (!f) {
        perror("fopen write");
        return 1;
    }
    if (fwrite(arr, sizeof(int32_t), n, f) != n) {
        perror("fwrite");
        fclose(f);
        return 1;
    }
    fclose(f);

    struct stat st;
    if (stat("p2e5.dat", &st) == 0) {
        printf("Taille du fichier: %lld octets\n", (long long)st.st_size);
        printf("sizeof(int32_t)*%zu = %zu\n", n, sizeof(int32_t) * n);
    }

    FILE *g = fopen("p2e5.dat", "rb");
    if (!g) {
        perror("fopen read");
        return 1;
    }
    int32_t buf[9];
    if (fread(buf, sizeof(int32_t), n, g) != n) {
        perror("fread");
        fclose(g);
        return 1;
    }
    fclose(g);

    printf("Valeurs lues:\n");
    for (size_t i = 0; i < n; ++i) {
        printf("%zu: %d\n", i, buf[i]);
    }
    return 0;
}
