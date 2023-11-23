#define _GNU_SOURCE
#include <dirent.h>     /* Defines DT_* constants */
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/syscall.h>

struct linux_dirent64 {
    ino64_t        d_ino;    /* 64-bit inode number */
    off64_t        d_off;    /* 64-bit offset to next structure */
    unsigned short d_reclen; /* Size of this dirent */
    unsigned char  d_type;   /* File type */
    char           d_name[]; /* Filename (null-terminated) */
};

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
    int fd;
    int nread;
    char buf[BUF_SIZE];
    struct linux_dirent64 *d;
    int bpos;
    char d_type;

    fd = open(argc > 1 ? argv[1] : ".", O_RDONLY | O_DIRECTORY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    for ( ; ; ) {
        nread = syscall(SYS_getdents64, fd, buf, BUF_SIZE);
        if (nread == -1) {
            perror("getdents64");
            exit(EXIT_FAILURE);
        }

        if (nread == 0)
            break;

        for (bpos = 0; bpos < nread;) {
            d = (struct linux_dirent64 *) (buf + bpos);
            printf("inode=%ld offset=%ld reclen=%u type=%d name=%s\n",
                    (long) d->d_ino, (long) d->d_off, d->d_reclen,
                    (int) d->d_type, d->d_name);
            bpos += d->d_reclen;
        }
    }

    exit(EXIT_SUCCESS);
}
