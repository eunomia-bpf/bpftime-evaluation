// SPDX-License-Identifier: (LGPL-2.1 OR BSD-2-Clause)
/* Copyright (c) 2020 Facebook */
#define _GNU_SOURCE

#include "gate.skel.h"
#include <bpf/bpf.h>
#include <bpf/libbpf.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <time.h>
#include <unistd.h>
#define warn(...) fprintf(stderr, __VA_ARGS__)

static int libbpf_print_fn(enum libbpf_print_level level, const char *format,
                           va_list args) {
  return vfprintf(stderr, format, args);
}

static volatile bool exiting = false;

static void sig_handler(int sig) { exiting = true; }

int main(int argc, char **argv) {
  struct gate_bpf *skel;
  int err;

  /* Set up libbpf errors and debug info callback */
  libbpf_set_print(libbpf_print_fn);

  /* Cleaner handling of Ctrl-C */
  signal(SIGINT, sig_handler);
  signal(SIGTERM, sig_handler);

  int pkey = pkey_alloc(0, PKEY_DISABLE_ACCESS);
  if (pkey == -1) {
    perror("pkey_alloc");
    return 1;
  }

  size_t pagesize = sysconf(_SC_PAGESIZE);

  /* Load and verify BPF application */
  skel = gate_bpf__open();
  if (!skel) {
    fprintf(stderr, "Failed to open and load BPF skeleton\n");
    return 1;
  }

  /* Load & verify BPF programs */
  err = gate_bpf__load(skel);
  if (err) {
    fprintf(stderr, "Failed to load and verify BPF skeleton\n");
    goto cleanup;
  }

  err = gate_bpf__attach(skel);
  if (err) {
    fprintf(stderr, "Failed to attach BPF skeleton\n");
    goto cleanup;
  }
  printf("map loc: %p\n", skel->maps.dir_map);
  printf("function ptr loc: %p\n", skel->progs.foo);

  //   void *ptr = mmap(NULL, pagesize, PROT_READ | PROT_WRITE,
  //                    MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  //   if (ptr == MAP_FAILED) {
  //     perror("mmap");
  //     return 1;
  //   }
  if (pkey_mprotect(skel->bss->buffer, sizeof(skel->bss->buffer),
                    PROT_READ | PROT_WRITE, pkey) == -1) {
    perror("pkey_mprotect");
    printf("%d\n", errno);
    return 1;
  }

  pkey_set(pkey, PKEY_DISABLE_ACCESS); // Disable access
  // Attempt to access the protected memory

  while (!exiting) {
    sleep(1);

    printf("loaded ebpf program...\n");
  }
cleanup:
  /* Clean up */
  gate_bpf__destroy(skel);
  pkey_set(pkey, 0); // Enable access

  pkey_free(pkey);

  return err < 0 ? -err : 0;
}
