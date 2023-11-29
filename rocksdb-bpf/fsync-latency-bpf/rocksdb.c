// SPDX-License-Identifier: (LGPL-2.1 OR BSD-2-Clause)
// Copyright (c) 2019 Facebook
// Copyright (c) 2020 Netflix
//
// Based on rocksdb(8) from BCC by Brendan Gregg and others.
// 14-Feb-2020   Brendan Gregg   Created this.
#include "rocksdb.h"
#include "rocksdb.skel.h"
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

static volatile sig_atomic_t exiting = 0;

static int libbpf_print_fn(enum libbpf_print_level level, const char *format,
                           va_list args) {
  return vfprintf(stderr, format, args);
}

static void sig_int(int signo) { exiting = 1; }

int main(int argc, char **argv) {
  LIBBPF_OPTS(bpf_object_open_opts, open_opts);
  // struct perf_buffer *pb = NULL;
  struct rocksdb_bpf *obj;
  int err;

  libbpf_set_print(libbpf_print_fn);

  obj = rocksdb_bpf__open_opts(&open_opts);
  if (!obj) {
    fprintf(stderr, "failed to open BPF object\n");
    return 1;
  }

  /* initialize global data (filtering options) */

  err = rocksdb_bpf__load(obj);
  if (err) {
    fprintf(stderr, "failed to load BPF object: %d\n", err);
    goto cleanup;
  }
	obj->bss->avg_latency.latency_to_write = 0;
	obj->bss->avg_latency.latency_to_fsync_exit = 0;
  err = rocksdb_bpf__attach(obj);
  if (err) {
    fprintf(stderr, "failed to attach BPF programs\n");
    goto cleanup;
  }

  if (signal(SIGINT, sig_int) == SIG_ERR) {
    fprintf(stderr, "can't set signal handler: %s\n", strerror(errno));
    err = 1;
    goto cleanup;
  }

  /* main: poll */
  while (!exiting) {
  }

  printf("avg latency_to_write: %lld\n", obj->bss->avg_latency.latency_to_write);
  printf("avg latency_to_fsync_exit: %lld\n",
         obj->bss->avg_latency.latency_to_fsync_exit);

cleanup:
  rocksdb_bpf__destroy(obj);

  return err != 0;
}
