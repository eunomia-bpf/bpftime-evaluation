#define BPF_NO_GLOBAL_DATA
#include <vmlinux.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>


SEC("uprobe/dummy")
int nginx(const char *str) {
  while (*str) {
    if (*str == 'X')
      return 1;
    str++;
  }
  return 0;
}

char LICENSE[] SEC("license") = "GPL";
