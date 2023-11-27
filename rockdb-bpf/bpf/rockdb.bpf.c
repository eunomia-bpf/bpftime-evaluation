/* SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2022, eunomia-bpf org
 * All rights reserved.
 */
#include <vmlinux.h>
#include <bpf/bpf_helpers.h>
#include "fs-cache.h"

SEC("tracepoint/syscalls/sys_enter_statfs")
int tracepoint__syscalls__sys_enter_statfs(struct trace_event_raw_sys_enter *ctx)
{
	// bpf_printk("trace_enter sys_enter_statfs\n");
	return 0;
}

char LICENSE[] SEC("license") = "GPL";
