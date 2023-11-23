#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    struct stat statbuf;
    int dfd; // Directory file descriptor
    int sflag = AT_SYMLINK_NOFOLLOW; // Do not follow symbolic links

    // Open the directory to get a file descriptor for it
    dfd = open(argc > 1 ? argv[1] : ".", O_RDONLY | O_DIRECTORY);
    if (dfd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Use fstatat to get information about a file within that directory
    if (fstatat(dfd, argc > 2 ? argv[2] : ".profile", &statbuf, sflag) == -1) {
        perror("newfstatat");
        close(dfd);
        exit(EXIT_FAILURE);
    }

    // Output some of the file information
    printf("File size: %lld bytes\n", (long long) statbuf.st_size);
    printf("File inode: %ld\n", (long) statbuf.st_ino);

    // Close the directory file descriptor
    close(dfd);
    return 0;
}
