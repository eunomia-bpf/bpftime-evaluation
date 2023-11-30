/* SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2022, eunomia-bpf org
 * All rights reserved.
 */
#include <vmlinux.h>
#include <bpf/bpf_core_read.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>

// unsigned long long last_ns = 0;
int successful_writeback_count = 0;
int current_pid = 0;

SEC("kprobe/file_write_and_wait_range")
int BPF_KPROBE(file_write_and_wait_range) {
  int pid = (bpf_get_current_pid_tgid() >> 32);
  if (pid != current_pid) {
    return 0;
  }
  // bpf_printk("__writeback_single_inode, %lu successful_writeback_count: %llu",
  //            i_ino, successful_writeback_count);
  // bpf_printk("data: %d, %d, %lu", data->pid, data->fd, data->i_ino);
  __sync_fetch_and_add(&successful_writeback_count, 1);
  return 0;
}

char LICENSE[] SEC("license") = "GPL";
