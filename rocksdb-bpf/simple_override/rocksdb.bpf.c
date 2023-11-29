/* SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2022, eunomia-bpf org
 * All rights reserved.
 */
#include <vmlinux.h>
#include "rocksdb.h"
#include <bpf/bpf_core_read.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>

// struct uring_check_data {
//   int pid;
//   int fd;
//   u32 i_ino;
// };

struct uring_queue {
  // make it easy to access for count
  u8 sync_count;
};

// struct {
//   __uint(type, BPF_MAP_TYPE_HASH);
//   __type(key, u32);                       // i_ino
//   __type(value, struct uring_check_data); // data
//   __uint(max_entries, 10000);
//   __uint(map_flags, 0);
// } bpftime_inode_map SEC(".maps");

// struct {
//   __uint(type, BPF_MAP_TYPE_HASH);
//   __type(key, u64);   // pid_tgid
//   __type(value, u64); // start time
//   __uint(max_entries, 10000);
//   __uint(map_flags, 0);
// } start_map SEC(".maps");


// SEC("uretprobe//root/zys/bpftime-evaluation/AisLSM/build/"
//     "librocksdb.so.7:_ZN7rocksdb6Urings14wait_for_queueEPNS_11uring_queueE")
// int BPF_UPROBE(_return_ZN7rocksdb6Urings14wait_for_queueEPNS_11uring_queueE,
//                struct uring_queue *uptr) {
//   u64 ns = bpf_ktime_get_ns();
//   bpf_printk("wait for queue count return latency: %lld", ns - last_ns);

//   return 0;
// }

SEC("uprobe//root/zys/bpftime-evaluation/AisLSM/build/"
    "librocksdb.so.7:_ZN7rocksdb6Urings14wait_for_queueEPNS_11uring_queueE")
int BPF_UPROBE(_enter_wait_for_queue,
               struct uring_queue *uptr) {
  if (!uptr) {
    bpf_printk("uptr is null");
    return 0;
  }
  bpf_override_return(0, 0);
  return 0;
}

char LICENSE[] SEC("license") = "GPL";
