#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    char buffer[100];
    int num_bytes;

    // Open a file
    int fd = open("test.txt", O_RDONLY);
    if (fd < 0) {
        return -1;
    }

    // Read from the file
    num_bytes = read(fd, buffer, sizeof(buffer));
    if (num_bytes < 0) {
        return -1;
    }
    syscall(1, "Intercepted syscall\n");

    // Write to standard output
    write(STDOUT_FILENO, buffer, num_bytes);

    // Close the file
    close(fd);

    return 0;
}
