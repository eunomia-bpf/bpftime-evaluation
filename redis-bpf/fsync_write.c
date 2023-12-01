#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

int test() {
    printf("test for fsync\n");
    int fd = open("temp.txt", O_WRONLY | O_CREAT, 0644);
    if (fd < 0) {
        perror("Failed to open file");
        return 1;
    }
    printf("fd: %d\n", fd);
    const char *data = "Hello";
    int count = 0;
    time_t start = time(NULL);

    while (time(NULL) - start < 3) {
        if (write(fd, data, 5) != 5) {
            perror("Failed to write");
            break;
        }
        if (fsync(fd) < 0) {
            perror("Failed to fsync");
            break;
        }
        count++;
    }

    close(fd);
    printf("Number of write and fsync calls in 3 seconds: %d\n", count);

    return 0;
}

__attribute_noinline__ void start() {
    printf("start\n");
}

int main() {
    start();
    while (1) {
        test();
        sleep(1);
    }
    return 0;
}
