/* SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2022, eunomia-bpf org
 * All rights reserved.
 */
#include <vmlinux.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include "rocksdb.h"

struct uring_check_id{
    u64 jobid;
    u32 inode;
    u32 hashed_inode;
};

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __type(key, u64); // pid
    __type(value, struct uring_check_id); // bitmap get murmurhash
    __uint(max_entries, 1000);
    __uint(map_flags, BPF_F_NO_PREALLOC);
} bpftime_uring_map SEC(".maps");

//SEC("uretprobe/rocksdb:_ZN7rocksdb6Urings14wait_for_queueEPNS_11uring_queueE")
//int wait_f(_ZN7rocksdb6Urings14wait_for_queueEPNS_11uring_queueE, int a, int b)
//{
//    bpf_printk("uprobed_sub ENTRY: a = %d, b = %d", a, b);
//    return 0;
//}

SEC("uprobe//lib/x86_64-linux-gnu/liburing.so.2:io_uring_wait_cqes")
int BPF_UPROBE(io_uring_wait_cqe)
{
    // if it's ready, set the iouring done flag and continue for this fd
    bpf_printk("io_uring_wait_cqes");
    return 0;
}

SEC("uprobe//lib/x86_64-linux-gnu/liburing.so.2:io_uring_get_sqe")
int BPF_UPROBE(io_uring_get_sqe)
{ // get fd submit id pid
    // insert job id pid to map
    bpf_printk("io_uring_get_sqe");
    return 0;
}

// 找到一个kernel
// trace点可以把 fd 的 inodemap拿到
//SEC("kretprobe:__writeback_single_inode")
//int check_done_ext_writeback(struct pt_regs *ctx)
//{
//    u32 pid = bpf_get_current_pid_tgid();
//    u64 *val, key = 0;
//    val = bpf_map_lookup_elem(&bpftime_uring_map, &key);
//    bpf_printk("__writeback_single_inode ENTRY: a = %d, b = %d", key, pid);
//
//    return 0;
//}

SEC("kretprobe/__writeback_single_inode")
int BPF_KRETPROBE(__writeback_single_inode)
{
    u64 pid = bpf_get_current_pid_tgid();
    bpf_printk("__writeback_single_inode");

    return 0;
}

char LICENSE[] SEC("license") = "GPL";
