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

struct uring_check_data {
  int pid;
  int fd;
  u32 i_ino;
};

struct uring_queue {
  // make it easy to access for count
  u8 sync_count;
};

struct {
  __uint(type, BPF_MAP_TYPE_HASH);
  __type(key, u32);                       // i_ino
  __type(value, struct uring_check_data); // data
  __uint(max_entries, 10000);
  __uint(map_flags, 0);
} bpftime_inode_map SEC(".maps");

struct {
  __uint(type, BPF_MAP_TYPE_HASH);
  __type(key, u64);   // pid_tgid
  __type(value, u64); // start time
  __uint(max_entries, 10000);
  __uint(map_flags, 0);
} start_map SEC(".maps");

// unsigned long long last_ns = 0;
unsigned long long successful_writeback_count = 0;

// SEC("uretprobe//lib/x86_64-linux-gnu/libc.so.6:putchar")
// int
// BPF_URETPROBE(_return_ZN7rocksdb6Urings14wait_for_queueEPNS_11uring_queueE,
//                int ret) {
//   u64 ns = bpf_ktime_get_ns();
//   bpf_printk("wait for queue count return latency: %lld", ns - last_ns);

//   return 0;
// }
// SEC("uprobe//root/zys/bpftime-evaluation/AisLSM/build/"
//     "librocksdb.so.7:_ZN7rocksdb6Urings14wait_for_queueEPNS_11uring_queueE")
SEC("uretprobe//lib/x86_64-linux-gnu/libc.so.6:putchar")
int BPF_UPROBE(_enter_ZN7rocksdb6Urings14wait_for_queueEPNS_11uring_queueE,
               struct uring_queue *uptr) {
  if (!uptr) {
    bpf_printk("uptr is null");
    return 0;
  }
  u8 count;
  bpf_probe_read_user(&count, sizeof(uptr->sync_count), &(uptr->sync_count));
  // u64 ns = bpf_ktime_get_ns();
  // last_ns = ns;
  // bpf_printk("wait for queue count enter: %d, writeback_count %d", count,
  // successful_writeback_count);
  if (successful_writeback_count <= 0 || successful_writeback_count < count) {
    return 0;
  }
  // bpf_printk("count: %d, successful_writeback_count: %llu", count,
  //            successful_writeback_count);
  successful_writeback_count -= count;
  bpf_override_return(0, 0);
  return 0;
}

SEC("uretprobe/AisLSM/build/"
    "librocksdb.so.7:bpftime_hook_function_for_submit_fd")
int BPF_UPROBE(bpftime_hook_function_for_submit_fd, int fd) {
  bpf_printk("fsync fd: %d", fd);
  struct task_struct *t;
  struct files_struct *f;
  struct fdtable *fdt;
  struct file **fdd;
  struct file *file;
  struct path path;
  struct dentry *dentry;
  struct qstr pathname;
  struct inode *f_inode;
  char filename[128];

  t = (struct task_struct *)bpf_get_current_task();
  f = BPF_CORE_READ(t, files);

  bpf_core_read(&fdt, sizeof(fdt), (void *)&f->fdt);
  int ret = bpf_core_read(&fdd, sizeof(fdd), (void *)&fdt->fd);
  if (ret) {
    bpf_printk("bpf_probe_read failed: %d\n", ret);
    return 0;
  }
  bpf_core_read(&file, sizeof(file), (void *)&fdd[fd]);
  if (!file) {
    bpf_printk("file is null");
    return 0;
  }
  bpf_core_read(&f_inode, sizeof(f_inode), (void *)&file->f_inode);
  bpf_core_read(&path, sizeof(path), (const void *)&file->f_path);

  dentry = path.dentry;
  bpf_core_read(&pathname, sizeof(pathname), (const void *)&dentry->d_name);
  bpf_core_read_str((void *)filename, sizeof(filename),
                    (const void *)pathname.name);
  if (!f_inode) {
    bpf_printk("inode is null");
    return 0;
  }
  long unsigned int i_ino = BPF_CORE_READ(f_inode, i_ino);
  bpf_printk("fsync filename: %s inode: %lu", filename, i_ino);
  struct uring_check_data data = {
      .pid = bpf_get_current_pid_tgid() >> 32,
      .fd = fd,
      .i_ino = i_ino,
  };
  bpf_map_update_elem(&bpftime_inode_map, &i_ino, &data, BPF_ANY);
  return 0;
}

// SEC("uprobe//root/zys/bpftime-evaluation/rocksdb/build/librocksdb.so.7:io_uring_submit")
// int BPF_UPROBE(io_uring_submit)
// {
//     // if it's ready, set the iouring done flag and continue for this fd
//     bpf_printk("io_uring_submit");
//     return 0;
// }

SEC("kprobe/__writeback_single_inode")
int BPF_KPROBE(__writeback_single_inode, struct inode *inode,
               struct writeback_control *wbc) {
  int pid = bpf_get_current_pid_tgid() >> 32;
  if (!inode) {
    // bpf_printk("inode is null");
    return 0;
  }
  long unsigned int i_ino = BPF_CORE_READ(inode, i_ino);
  struct uring_check_data *data =
      bpf_map_lookup_elem(&bpftime_inode_map, &i_ino);
  if (!data) {
    // bpf_printk("data is null");
    return 0;
  }
  bpf_printk("__writeback_single_inode, %lu successful_writeback_count: %llu",
             i_ino, successful_writeback_count);
  bpf_printk("data: %d, %d, %lu", data->pid, data->fd, data->i_ino);
  successful_writeback_count++;
  return 0;
}

char LICENSE[] SEC("license") = "GPL";
