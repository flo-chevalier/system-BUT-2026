/*
 p2e10.c - Créer une arborescence + liens
 Usage: p2e10 rep_name nb_d nb_f
*/
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s rep_name nb_d nb_f\n", argv[0]);
        return 2;
    }
    const char *rep = argv[1];
    int nb_d = atoi(argv[2]);
    int nb_f = atoi(argv[3]);
    if (nb_d < 1 || nb_f < 1) {
        fprintf(stderr, "nb_d and nb_f must be >= 1\n");
        return 2;
    }
    if (mkdir(rep, 0755) != 0 && errno != EEXIST) {
        perror("mkdir root");
        return 1;
    }

    char path[4096];
    for (int d = 1; d <= nb_d; ++d) {
        snprintf(path, sizeof(path), "%s/sous_rep_%d", rep, d);
        if (mkdir(path, 0755) != 0 && errno != EEXIST) {
            perror("mkdir sub");
            return 1;
        }
        for (int f = 1; f <= nb_f; ++f) {
            snprintf(path,
                     sizeof(path),
                     "%s/sous_rep_%d/sous_rep_%d_fichier_%d.txt",
                     rep,
                     d,
                     d,
                     f);
            int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                perror("open create file");
                return 1;
            }
            close(fd);
        }
    }
    /* liens au niveau racine */
    char target[4096], linkp[4096];
    snprintf(target, sizeof(target), "sous_rep_1/sous_rep_1_fichier_1.txt");
    snprintf(linkp,
             sizeof(linkp),
             "%s/lien_physique_vers_sous_rep_1_fichier_1.txt",
             rep);
    char t2[4096], linkp2[4096];
    snprintf(t2, sizeof(t2), "sous_rep_1/sous_rep_1_fichier_2.txt");
    snprintf(linkp2,
             sizeof(linkp2),
             "%s/lien_symbolique_vers_sous_rep_1_fichier_2.txt",
             rep);

    /* hard link: newpath must be in same filesystem; target is relative to rep
     */
    char oldpath[4096];
    snprintf(oldpath,
             sizeof(oldpath),
             "%s/%s",
             rep,
             "sous_rep_1/sous_rep_1_fichier_1.txt");
    if (link(oldpath, linkp) != 0) {
        perror("link (hard)"); /* continue */
    }

    /* symlink: target relative path from the link */
    if (symlink(t2, linkp2) != 0) {
        perror("symlink"); /* continue */
    }

    return 0;
}
