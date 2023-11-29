#define BPF_NO_GLOBAL_DATA
#include <vmlinux.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>


SEC("uprobe/dummy")
int nginx(void* ctx)
{
	bpf_printk("Triggered!");
	return 0;
}

char LICENSE[] SEC("license") = "GPL";
