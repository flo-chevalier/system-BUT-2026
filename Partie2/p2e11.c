/*
 p2e11.c - Copier un fichier (POSIX)
 Usage: p2e11 SOURCE DEST
*/
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

ssize_t write_full(int fd, const void *buf, size_t count) {
    size_t done = 0;
    while (done < count) {
        ssize_t w = write(fd, (const char *)buf + done, count - done);
        if (w > 0) {
            done += (size_t)w;
        } else if (w < 0 && errno == EINTR) {
            continue;
        } else {
            return -1;
        }
    }
    return (ssize_t)done;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s SOURCE DEST\n", argv[0]);
        return EXIT_FAILURE;
    }
    const char *src = argv[1];
    const char *dst = argv[2];

    int fd_src = open(src, O_RDONLY);
    if (fd_src < 0) {
        perror("open source");
        return EXIT_FAILURE;
    }

    int fd_dst = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_dst < 0) {
        perror("open dest");
        close(fd_src);
        return EXIT_FAILURE;
    }

    char buf[8192];
    ssize_t r;
    while ((r = read(fd_src, buf, sizeof(buf))) > 0) {
        if (write_full(fd_dst, buf, (size_t)r) != r) {
            perror("write_full");
            close(fd_src);
            close(fd_dst);
            return EXIT_FAILURE;
        }
    }
    if (r < 0) {
        perror("read");
        close(fd_src);
        close(fd_dst);
        return EXIT_FAILURE;
    }

    close(fd_src);
    close(fd_dst);
    return EXIT_SUCCESS;
}
