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

SEC("uprobe/main:foo")
int BPF_UPROBE(foo, int a, int b) {
  bpf_printk("a = %d, b = %d\n", a, b);
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
