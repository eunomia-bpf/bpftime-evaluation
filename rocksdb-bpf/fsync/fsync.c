#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
  int fileDescriptor;
  const char *data = "Sample data to be written to the file.\n";

  // Open the file for writing
  fileDescriptor = open("example.txt", O_WRONLY | O_CREAT, 0644);
  if (fileDescriptor < 0) {
    perror("open");
    return 1;
  }

  while (1) {
    // Write data to the file
    if (write(fileDescriptor, data, strlen(data)) != strlen(data)) {
      perror("write");
      close(fileDescriptor);
      return 1;
    }

    // Use fsync to flush the data to disk
    if (fsync(fileDescriptor) < 0) {
      perror("fsync");
      close(fileDescriptor);
      return 1;
    }
  }

  // Close the file
  close(fileDescriptor);
  return 0;
}
