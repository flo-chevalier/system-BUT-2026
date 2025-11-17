// p3e1.c — Démonstration de stack overflow (pile d'appels)
// Compile: gcc -std=c2x -Wall -Wextra -pedantic -g p3e1.c -o p3e1
// With sanitizers (optional): -fsanitize=address,undefined
// -fno-sanitize-recover=all
#define _POSIX_C_SOURCE 200809L
#include <stdint.h>
#include <stdio.h>

static void recurse(size_t depth) {
    // "Beaucoup" de mémoire sur la pile
    char buf[1024];
    printf("depth=%zu, &buf=%p\n", depth, (void *)buf);
    recurse(depth + 1); // pas de condition d'arrêt -> débordement de pile
}

int main(void) {
    recurse(0);
    return 0;
}
