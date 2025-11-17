/*
Attention, ce code est bugué !
Trouvez les erreurs et résolvez les,
gardez en commentaire les erreurs et
expliquez d'où vient l'erreur et
comment l'empêcher
*/
#include <stdio.h>
#include <stdlib.h>

double average(int *notes, size_t n) {
    long sum = 0;
    // lecture hors limites
    for (size_t i = 0; i <= n; ++i) {
        sum += notes[i];
    }
    // le free est fait par la fonction avant l'utilisation dans le main
    free(notes);
    return (double)sum / (double)n;
}

int main(void) {
    size_t n = 3; // soit il faut n = 4
    int *g = malloc(n * sizeof *g);
    if (!g)
        return 1;
    g[0] = 12;
    g[1] = 14;
    g[2] = 16;
    g[3] = 18; // soit il ne faut pas du g[3]

    double avg = average(g, n);
    printf("Moyenne: %.2f\n", avg);

    puts("Voici les notes :");
    // lecture hors limites
    for (size_t i = 0; i <= n; ++i) {
        printf("%d\t", g[i]);
    }

    return 0;
}
