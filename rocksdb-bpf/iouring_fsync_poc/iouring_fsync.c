/* SPDX-License-Identifier: MIT */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include "liburing.h"
// SPDX-License-Identifier: (LGPL-2.1 OR BSD-2-Clause)
// Copyright (c) 2019 Facebook
// Copyright (c) 2020 Netflix
//
// Based on rocksdb(8) from BCC by Brendan Gregg and others.
// 14-Feb-2020   Brendan Gregg   Created this.
#include "iouring_fsync.skel.h"
#include <argp.h>
#include <bpf/bpf.h>
#include <bpf/libbpf.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

static unsigned long runtime_ms = 3000;
static unsigned long batch_size = 5;
bool wait_kernel = true;

static volatile sig_atomic_t exiting = 0;

static int libbpf_print_fn(enum libbpf_print_level level, const char *format,
                           va_list args) {
  return 0; vfprintf(stderr, format, args);
}

static void sig_int(int signo) { exiting = 1; }

static unsigned long gettimeofday_ms(void) {
  struct timeval tv;

  gettimeofday(&tv, NULL);
  return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

static int submit_io_uring_write(struct io_uring *ring, int fd, char *buf,
                                 size_t size) {
  struct io_uring_sqe *sqe;

  sqe = io_uring_get_sqe(ring);
  if (!sqe) {
    fprintf(stderr, "get sqe failed\n");
    return 1;
  }

  io_uring_prep_write(sqe, fd, buf, size, 0);
  sqe->user_data = 1;

  return 0;
}

static int submit_io_uring_fsync(struct io_uring *ring, int fd) {
  struct io_uring_sqe *sqe;

  sqe = io_uring_get_sqe(ring);
  if (!sqe) {
    fprintf(stderr, "get sqe failed\n");
    return 1;
  }

  io_uring_prep_fsync(sqe, fd, 0);
  sqe->user_data = 2;

  return 0;
}

int test(struct iouring_fsync_bpf *obj) {
  unsigned long tstop;
  unsigned long nr_reqs = 0;
  struct io_uring ring;
  struct io_uring_cqe *cqe;
  int ret, fd;
  char buf[256], template[] = "/tmp/io_uring_test_XXXXXX";

  // Generate some random data
  for (int i = 0; i < sizeof(buf); i++) {
    buf[i] = rand() % 256;
  }

  fd = mkstemp(template);
  if (fd < 0) {
    perror("mkstemp");
    return 1;
  }

  ret = io_uring_queue_init(1024, &ring, IORING_SETUP_SINGLE_ISSUER);
  if (ret) {
    fprintf(stderr, "ring setup failed: %d\n", ret);
    close(fd);
    unlink(template);
    return 1;
  }
  obj->bss->successful_writeback_count = 0;
  tstop = gettimeofday_ms() + runtime_ms;
  do {
    int sync_count = 0;
    for (int i = 0; i < batch_size; i++) {
      ret = submit_io_uring_write(&ring, fd, buf, sizeof(buf));
      if (ret) {
        fprintf(stderr, "submit_io_uring_write failed\n");
        break;
      }
      ret = submit_io_uring_fsync(&ring, fd);
      if (ret) {
        fprintf(stderr, "submit_io_uring_fsync failed\n");
        break;
      }
      sync_count++;
    }
    ret = io_uring_submit(&ring);
    if (ret < 0) {
      fprintf(stderr, "sqe submit failed: %d\n", ret);
      return 1;
    }
    // Wait for completion of both write and fsync
    if (wait_kernel) {
      for (int i = 0; i < sync_count; i++) {
        // if kernel has successfully writeback, then decrease the counter and
        // not wait
        if (obj->bss->successful_writeback_count > 0) {
          __sync_fetch_and_add(&obj->bss->successful_writeback_count, -1);
          // printf("successful_writeback_count: %lu\n",
          //        obj->bss->successful_writeback_count);
          ret = 0;
        } else {
          // wait for kernel to writeback
          ret = io_uring_wait_cqe(&ring, &cqe);
          if (ret < 0) {
            fprintf(stderr, "wait completion %d\n", ret);
            break;
          }
          io_uring_cqe_seen(&ring, cqe);
          ret = io_uring_wait_cqe(&ring, &cqe);
          if (ret < 0) {
            fprintf(stderr, "wait completion %d\n", ret);
            break;
          }
          io_uring_cqe_seen(&ring, cqe);
        }
      }
    } else {
      // wait for fsync and write completion
      for (int i = 0; i < sync_count * 2; i++) {
        ret = io_uring_wait_cqe(&ring, &cqe);
        if (ret < 0) {
          fprintf(stderr, "wait completion %d\n", ret);
          break;
        }
        io_uring_cqe_seen(&ring, cqe);
      }
    }
    nr_reqs += batch_size;
  } while (gettimeofday_ms() < tstop && ret == 0);

  fprintf(stderr, "Completed operations: %lu\n", nr_reqs);
  close(fd);
  unlink(template);
  return ret;
}

int main(int argc, char **argv) {
  LIBBPF_OPTS(bpf_object_open_opts, open_opts);
  // struct perf_buffer *pb = NULL;
  struct iouring_fsync_bpf *obj;
  int err;

  libbpf_set_print(libbpf_print_fn);

  obj = iouring_fsync_bpf__open_opts(&open_opts);
  if (!obj) {
    fprintf(stderr, "failed to open BPF object\n");
    return 1;
  }

  /* initialize global data (filtering options) */

  err = iouring_fsync_bpf__load(obj);
  if (err) {
    fprintf(stderr, "failed to load BPF object: %d\n", err);
    goto cleanup;
  }

  err = iouring_fsync_bpf__attach(obj);
  if (err) {
    fprintf(stderr, "failed to attach BPF programs\n");
    goto cleanup;
  }
  obj->bss->current_pid = getpid();
  obj->bss->successful_writeback_count = 0;
  if (signal(SIGINT, sig_int) == SIG_ERR) {
    fprintf(stderr, "can't set signal handler: %s\n", strerror(errno));
    err = 1;
    goto cleanup;
  }
  printf("Starting test\n");
  for (int i = 0; i < 10 && !exiting; i++) {
    batch_size = 1 << i;
    test(obj);
    printf("batch_size = %lu completed\n", batch_size);
  }

cleanup:
  iouring_fsync_bpf__destroy(obj);

  return err != 0;
}
