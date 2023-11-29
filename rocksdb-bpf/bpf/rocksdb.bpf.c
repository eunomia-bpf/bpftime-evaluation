/* SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2022, eunomia-bpf org
 * All rights reserved.
 */
#include <vmlinux.h>
#include "rocksdb.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_core_read.h>

struct uring_check_id {
  u64 jobid;
  u32 inode;
  u32 hashed_inode;
};

struct uring_queue {
  // make it easy to access for count
  u8 sync_count;
};

struct {
  __uint(type, BPF_MAP_TYPE_HASH);
  __type(key, u64);                     // pid
  __type(value, struct uring_check_id); // bitmap get murmurhash
  __uint(max_entries, 1000);
  __uint(map_flags, BPF_F_NO_PREALLOC);
} bpftime_uring_map SEC(".maps");

struct {
  __uint(type, BPF_MAP_TYPE_HASH);
  __type(key, u64);   // pid_tgid
  __type(value, u64); // start time
  __uint(max_entries, 1000);
  __uint(map_flags, 0);
} start_map SEC(".maps");

unsigned long long last_ns = 0;

SEC("uretprobe//root/zys/bpftime-evaluation/rocksdb/build/"
    "librocksdb.so.7:_ZN7rocksdb6Urings14wait_for_queueEPNS_11uring_queueE")
int BPF_UPROBE(_return_ZN7rocksdb6Urings14wait_for_queueEPNS_11uring_queueE,
               struct uring_queue *uptr) {
  u64 ns = bpf_ktime_get_ns();
  bpf_printk("wait for queue count return latency: %lld", ns - last_ns);

  return 0;
}

SEC("uprobe//root/zys/bpftime-evaluation/rocksdb/build/"
    "librocksdb.so.7:_ZN7rocksdb6Urings14wait_for_queueEPNS_11uring_queueE")
int BPF_UPROBE(_enter_ZN7rocksdb6Urings14wait_for_queueEPNS_11uring_queueE,
               struct uring_queue *uptr) {
  if (!uptr) {
    bpf_printk("uptr is null");
    return 0;
  }
  u8 count;
  bpf_probe_read_user(&count, sizeof(uptr->sync_count), &(uptr->sync_count));
  u64 ns = bpf_ktime_get_ns();
  bpf_printk("wait for queue count enter: %d, %llu", count, ns);
    last_ns = ns;
  return 0;
}

SEC("uretprobe//root/zys/bpftime-evaluation/rocksdb/build/"
    "librocksdb.so.7:bpftime_hook_function_for_submit_fd")
int BPF_UPROBE(bpftime_hook_function_for_submit_fd, int fd) {
  bpf_printk("fsync fd: %d", fd);
  struct task_struct* t;
    struct files_struct* f;
    struct fdtable* fdt;
    struct file** fdd;
    struct file* file;
    struct path path;
    struct dentry* dentry;
    struct qstr pathname;
    char filename[128];

    t = (struct task_struct*)bpf_get_current_task();
    f = BPF_CORE_READ(t, files);

    bpf_core_read(&fdt, sizeof(fdt), (void*)&f->fdt);
    int ret = bpf_core_read(&fdd, sizeof(fdd), (void*)&fdt->fd); 
    if (ret) {
        bpf_printk("bpf_probe_read failed: %d\n", ret);
        return 0;
    }
    bpf_core_read(&file, sizeof(file), (void*)&fdd[fd]);
    bpf_core_read(&path, sizeof(path), (const void*)&file->f_path);

    dentry = path.dentry;
    bpf_core_read(&pathname, sizeof(pathname), (const void*)&dentry->d_name);
    bpf_core_read_str((void*)filename, sizeof(filename), (const void*)pathname.name);
    bpf_printk("fsync filename: %s", filename);
  return 0;
}

// SEC("uprobe//root/zys/bpftime-evaluation/rocksdb/build/librocksdb.so.7:io_uring_submit")
// int BPF_UPROBE(io_uring_submit)
// {
//     // if it's ready, set the iouring done flag and continue for this fd
//     bpf_printk("io_uring_submit");
//     return 0;
// }

// 找到一个kernel
// trace点可以把 fd 的 inodemap拿到
// SEC("kretprobe:__writeback_single_inode")
// int check_done_ext_writeback(struct pt_regs *ctx)
//{
//    u32 pid = bpf_get_current_pid_tgid();
//    u64 *val, key = 0;
//    val = bpf_map_lookup_elem(&bpftime_uring_map, &key);
//    bpf_printk("__writeback_single_inode ENTRY: a = %d, b = %d", key, pid);
//
//    return 0;
//}

SEC("kretprobe/__writeback_single_inode")
int BPF_KPROBE(__writeback_single_inode, struct inode *inode, struct writeback_control *wbc) {
  int pid = bpf_get_current_pid_tgid() >> 32;
  bpf_printk("__writeback_single_inode, %p", inode);

  return 0;
}

char LICENSE[] SEC("license") = "GPL";
