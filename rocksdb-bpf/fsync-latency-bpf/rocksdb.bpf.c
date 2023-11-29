/* SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2022, eunomia-bpf org
 * All rights reserved.
 */
#include <vmlinux.h>
#include "rocksdb.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>

struct avg_latency_t avg_latency = {0};

struct {
  __uint(type, BPF_MAP_TYPE_HASH);
  __type(key, u64);   // pid_tgid
  __type(value, u64); // start time
  __uint(max_entries, 1000);
  __uint(map_flags, 0);
} start_map SEC(".maps");

SEC("kprobe/__writeback_single_inode")
int BPF_KRETPROBE(__writeback_single_inode) {
  u64 pid_tgid = bpf_get_current_pid_tgid();
  u64 *start_ns = bpf_map_lookup_elem(&start_map, &pid_tgid);
  if (start_ns == NULL) {
    return 0;
  }
  u64 ns = bpf_ktime_get_ns();
  s64 latency = ns - *start_ns;
  u64 count = avg_latency.count;
  avg_latency.latency_to_write +=
      (latency - avg_latency.latency_to_write) / (count + 1);
  avg_latency.count += 1;
  bpf_printk("__writeback_single_inode latency: %lls", latency);
  return 0;
}

SEC("tracepoint/syscalls/sys_enter_fsync")
int tracepoint__syscalls__sys_enter_statfs(
    struct trace_event_raw_sys_enter *ctx) {
  // bpf_printk("trace_enter sys_enter_statfs\n");
  u64 pid_tgid = bpf_get_current_pid_tgid();
  u64 ns = bpf_ktime_get_ns();
  bpf_map_update_elem(&start_map, &pid_tgid, &ns, 0);
  return 0;
}

SEC("tracepoint/syscalls/sys_exit_fsync")
int tracepoint__syscalls__sys_exit_statfs(
    struct trace_event_raw_sys_exit *ctx) {
  // bpf_printk("trace_enter sys_enter_statfs\n");
  u64 pid_tgid = bpf_get_current_pid_tgid();
  u64 *start_ns = bpf_map_lookup_elem(&start_map, &pid_tgid);
  if (start_ns == NULL) {
    return 0;
  }
  u64 ns = bpf_ktime_get_ns();
  s64 latency = ns - *start_ns;
  u64 count = avg_latency.count;
  avg_latency.latency_to_fsync_exit =
      (latency - avg_latency.latency_to_fsync_exit) / (count + 1);
  avg_latency.count += 1;
  bpf_printk("fsync latency: %lls, count %llu", latency, count);
  bpf_printk("avg fsync latency: %lls", avg_latency.latency_to_fsync_exit);
  bpf_map_delete_elem(&start_map, &pid_tgid);
  return 0;
}

char LICENSE[] SEC("license") = "GPL";
