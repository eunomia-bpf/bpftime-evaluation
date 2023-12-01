/* SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2022, eunomia-bpf org
 * All rights reserved.
 */
 
#define BPF_NO_GLOBAL_DATA
#include <vmlinux.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>

// pid & fd for all open dir related fds
struct {
  __uint(type, BPF_MAP_TYPE_HASH);
  __uint(max_entries, 10240);
  __type(key, u64);
  __type(value, u32);
} dir_map SEC(".maps");

struct {
        __uint(type, BPF_MAP_TYPE_ARRAY);
        __type(key, u32);
        __type(value, long);
        __uint(max_entries, 256);
} dir1_map SEC(".maps");

char buffer[123];

SEC("uprobe/./main:foo1")
int BPF_UPROBE(foo, int a, int b) {
  bpf_printk("foo1: %d, %d\n", a, b);
  u64 id = bpf_get_current_pid_tgid();
  if (bpf_map_lookup_elem(&dir_map, &id)) {
    bpf_printk("found\n");
    bpf_map_delete_elem(&dir_map, &id);
  } else {
    bpf_printk("not found\n");
    bpf_map_update_elem(&dir_map, &id, &a, 0);
  }
  return 0;
}

char LICENSE[] SEC("license") = "GPL";
