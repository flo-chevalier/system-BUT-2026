/*
 p2e4.c - I/O texte POSIX (bas niveau)
 Écrit nazg.txt avec write() puis lit son contenu avec read().
*/
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

ssize_t write_full(int fd, const void *buf, size_t count) {
    size_t done = 0;
    while (done < count) {
        ssize_t w = write(fd, (const char *)buf + done, count - done);
        if (w > 0)
            done += (size_t)w;
        else if (w < 0 && errno == EINTR)
            continue;
        else
            return -1;
    }
    return (ssize_t)done;
}

int main(void) {
    const char *fname = "nazg.txt";
    const char *text = "Ash nazg durbatulûk,\n"
                       "ash nazg gimbatul,\n"
                       "ash nazg thrakatulûk agh burzum-ishi krimpatul\n";

    int fd = open(fname, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open for write");
        return 1;
    }

    if (write_full(fd, text, strlen(text)) < 0) {
        perror("write");
        close(fd);
        return 1;
    }
    close(fd);

    fd = open(fname, O_RDONLY);
    if (fd < 0) {
        perror("open for read");
        return 1;
    }

    char buf[256];
    ssize_t r;
    while ((r = read(fd, buf, sizeof(buf))) > 0) {
        ssize_t w = write(1, buf, (size_t)r); // write to stdout (fd 1)
        if (w < 0) {
            perror("write to stdout");
            close(fd);
            return 1;
        }
    }
    if (r < 0) {
        perror("read");
        close(fd);
        return 1;
    }
    close(fd);
    return 0;
}
