#include "liburing.h"
#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

// static int submit_io_uring_write(struct io_uring *ring, int fd, char *buf,
//                                  size_t size) {
//   struct io_uring_sqe *sqe;

//   sqe = io_uring_get_sqe(ring);
//   if (!sqe) {
//     fprintf(stderr, "get sqe failed\n");
//     return 1;
//   }

//   io_uring_prep_write(sqe, fd, buf, size, -1);
//   sqe->user_data = 1;

//   return 0;
// }

// static int submit_io_uring_fsync(struct io_uring *ring, int fd) {
//   struct io_uring_sqe *sqe;

//   sqe = io_uring_get_sqe(ring);
//   if (!sqe) {
//     fprintf(stderr, "get sqe failed\n");
//     return 1;
//   }

//   io_uring_prep_fsync(sqe, fd, 0);
//   sqe->user_data = 2;

//   return 0;
// }

// static int io_uring_init(struct io_uring *ring) {
//   int ret = io_uring_queue_init(1024, ring, IORING_SETUP_SINGLE_ISSUER);
//   if (ret) {
//     fprintf(stderr, "ring setup failed: %d\n", ret);
//     return 1;
//   }
//   return 0;
// }

// static int io_uring_wait_and_seen(struct io_uring *ring,
//                                   struct io_uring_cqe *cqe) {
//   int ret = io_uring_wait_cqe(ring, &cqe);
//   if (ret < 0) {
//     fprintf(stderr, "wait completion %d\n", ret);
//     return ret;
//   }
//   io_uring_cqe_seen(ring, cqe);
//   return 0;
// }

// struct io_uring ring;

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
    // struct io_uring_cqe *cqe = 0;
    // submit_io_uring_write(&ring, fd, data, 5);
    // io_uring_submit(&ring);
    // io_uring_wait_and_seen(&ring, cqe);
    // submit_io_uring_fsync(&ring, fd);
    // io_uring_submit(&ring);
    // io_uring_wait_and_seen(&ring, cqe);
    count++;
  }

  close(fd);
  printf("Number of write and fsync calls in 3 seconds: %d\n", count);

  return 0;
}

__attribute_noinline__ void start() {
  printf("start\n");
//   io_uring_init(&ring);
}

int main() {
  start();
  while (1) {
    test();
    sleep(1);
  }
  return 0;
}
