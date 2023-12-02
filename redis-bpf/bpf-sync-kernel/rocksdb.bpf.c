/* SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2022, eunomia-bpf org
 * All rights reserved.
 */
#define BPF_NO_GLOBAL_DATA
#include <vmlinux.h>
#include "rocksdb.h"
#include <bpf/bpf_core_read.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>

#define EXTENDED_HELPER_IOURING_INIT 1006
#define EXTENDED_HELPER_IOURING_SUBMIT_WRITE 1007
#define EXTENDED_HELPER_IOURING_SUBMIT_FSYNC 1008
#define EXTENDED_HELPER_IOURING_WAIT_AND_SEEN 1009
#define EXTENDED_HELPER_IOURING_SUBMIT 1010

static long (*io_uring_init_global)(void) = (void *)
    EXTENDED_HELPER_IOURING_INIT;
static long (*io_uring_submit_write)(int fd, char *buf,
                                     unsigned long long size) = (void *)
    EXTENDED_HELPER_IOURING_SUBMIT_WRITE;
static long (*io_uring_submit_fsync)(int fd) = (void *)
    EXTENDED_HELPER_IOURING_SUBMIT_FSYNC;
static long (*io_uring_wait_and_seen)(void) = (void *)
    EXTENDED_HELPER_IOURING_WAIT_AND_SEEN;
static long (*io_uring_submit)(void) = (void *)EXTENDED_HELPER_IOURING_SUBMIT;

// unsigned long long last_ns = 0;
int successful_writeback_count = 0;
int current_pid = 0;

SEC("uprobe")
int BPF_UPROBE(start_config, int __fd) {
  bpf_printk("start called and init io_uring\n");
  io_uring_init_global();
  current_pid = (bpf_get_current_pid_tgid() >> 32);
  successful_writeback_count = -1;
  return 0;
}
// SEC("uprobe//root/zys/bpftime-evaluation/redis/src/"
//     "redis-server:initServerConfig")
// int BPF_UPROBE(start, int __fd) {
//   bpf_printk("start called and init io_uring\n");
//   current_pid = (bpf_get_current_pid_tgid() >> 32);
//   return 0;
// }

// SEC("uprobe//root/zys/bpftime-evaluation/redis/src/"
//     "redis-server:connTypeInitialize")
// int BPF_UPROBE(connTypeInitialize_start, int __fd) {
//   bpf_printk("start called and init io_uring\n");
//   //  current_pid = (bpf_get_current_pid_tgid() >> 32);
//   return 0;
// }

// struct uring_check_data {
//   int pid;
//   int fd;
//   u32 i_ino;
// };

// struct {
//   __uint(type, BPF_MAP_TYPE_HASH);
//   __type(key, struct file*);                       // i_ino
//   __type(value, u32); // fd
//   __uint(max_entries, 10000);
//   __uint(map_flags, 0);
// } bpftime_inode_map SEC(".maps");

// SEC("tracepoint/syscalls/sys_enter_write")
// int tracepoint__syscalls__sys_enter_write(struct trace_event_raw_sys_enter
// *ctx) {
//   int pid = (bpf_get_current_pid_tgid() >> 32);
//   // bpf_printk("pid: %d, current_pid: %d", pid, current_pid);
//   if (pid != current_pid) {
//     return 0;
//   }
//   int fd = ctx->args[0];
//   bpf_printk("write called, fd: %d", fd);
//   struct task_struct *t;
//   struct files_struct *f;
//   struct fdtable *fdt;
//   struct file **fdd;
//   struct file *file;
//   struct path path;
//   struct dentry *dentry;
//   struct qstr pathname;
//   struct inode *f_inode;
//   char filename[128];

//   t = (struct task_struct *)bpf_get_current_task();
//   f = BPF_CORE_READ(t, files);

//   bpf_core_read(&fdt, sizeof(fdt), (void *)&f->fdt);
//   int ret = bpf_core_read(&fdd, sizeof(fdd), (void *)&fdt->fd);
//   if (ret) {
//     bpf_printk("bpf_probe_read failed: %d\n", ret);
//     return 0;
//   }
//   bpf_core_read(&file, sizeof(file), (void *)&fdd[fd]);
//   if (!file) {
//     bpf_printk("file is null");
//     return 0;
//   }
//   bpf_map_update_elem(&bpftime_inode_map, &file, &fd, BPF_ANY);
//   return 0;
// }

SEC("uprobe")
int BPF_UPROBE(start_fsync, int __fd) {
  // bpf_printk("fsync called and submit io_uring\n");
  if (successful_writeback_count == 0) {
    // bpf_printk("fsync called and no writeback is completed");
    io_uring_wait_and_seen();
  } else {
    // bpf_printk("fsync called and %d writeback is completed",
    //            successful_writeback_count);
  }
  successful_writeback_count = 0;
  io_uring_submit_fsync(__fd);
  io_uring_submit();
  // else the kernel should have completed the sync and add successful_writeback_count
  bpf_override_return(0, 0);
  return 0;
}

SEC("kretprobe/file_write_and_wait_range")
int BPF_KPROBE(file_write_and_wait_range, struct file *file, loff_t start,
               loff_t end) {
  int pid = (bpf_get_current_pid_tgid() >> 32);
  // bpf_printk("pid: %d, current_pid: %d", pid, current_pid);
  if (pid != current_pid) {
    return 0;
  }
  // bpf_printk("file_write_and_wait_range called");
  // bpf_printk("data: %d, %d, %lu", data->pid, data->fd, data->i_ino);
  // struct file** f_ptr = bpf_map_lookup_elem(&bpftime_inode_map, &file);
  // if (!f_ptr) {
  //   bpf_printk("f_ptr is null");
  //   return 0;
  // }
  // bpf_printk("f_ptr: %p", f_ptr);
  __sync_fetch_and_add(&successful_writeback_count, 1);
  return 0;
}

char LICENSE[] SEC("license") = "GPL";
