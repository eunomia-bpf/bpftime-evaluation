/* SPDX-License-Identifier: MIT */
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "liburing.h"

static unsigned long runtime_ms = 10000;

static unsigned long gettimeofday_ms(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

static int submit_io_uring_write(struct io_uring *ring, int fd, char *buf, size_t size)
{
    struct io_uring_sqe *sqe;
    int ret;

    sqe = io_uring_get_sqe(ring);
    if (!sqe) {
        fprintf(stderr, "get sqe failed\n");
        return 1;
    }

    io_uring_prep_write(sqe, fd, buf, size, 0);
    sqe->user_data = 1;

    ret = io_uring_submit(ring);
    if (ret < 0) {
        fprintf(stderr, "sqe submit failed: %d\n", ret);
        return 1;
    }

    return 0;
}

static int submit_io_uring_fsync(struct io_uring *ring, int fd)
{
    struct io_uring_sqe *sqe;
    int ret;

    sqe = io_uring_get_sqe(ring);
    if (!sqe) {
        fprintf(stderr, "get sqe failed\n");
        return 1;
    }

    io_uring_prep_fsync(sqe, fd, 0);
    sqe->user_data = 2;

    ret = io_uring_submit(ring);
    if (ret < 0) {
        fprintf(stderr, "sqe submit failed: %d\n", ret);
        return 1;
    }

    return 0;
}

int main(void)
{
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

    tstop = gettimeofday_ms() + runtime_ms;
    do {
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

        // Wait for completion of both write and fsync
        for (int i = 0; i < 2; i++) {
            ret = io_uring_wait_cqe(&ring, &cqe);
            if (ret < 0) {
                fprintf(stderr, "wait completion %d\n", ret);
                break;
            }
            io_uring_cqe_seen(&ring, cqe);
        }

        nr_reqs++;
    } while (gettimeofday_ms() < tstop && ret == 0);

    fprintf(stderr, "Completed operations: %lu\n", nr_reqs);
    close(fd);
    unlink(template);
    return ret;
}
