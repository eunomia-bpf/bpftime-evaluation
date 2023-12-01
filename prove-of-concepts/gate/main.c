#define _GNU_SOURCE
#include <bpf/bpf.h>
#include <bpf/libbpf.h>
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

struct fake_pt_regs {
  int a;
  int b;
};

// function to hook
int foo1(int a, int b) {
  printf("%d, %d\n", a, b);
  return a + b;
}
// __asm__(".global foo1\n"
//     "foo1:\n"
//     "push   %rbp\n"
//     "mov    %rsp,%rbp\n"
//     "sub    $0x10,%rsp\n"
//     "mov    %edi,-0x4(%rbp)\n"
//     "mov    %esi,-0x8(%rbp)\n"
//     "mov    -0x8(%rbp),%edx\n"
//     "mov    -0x4(%rbp),%eax\n"
//     "mov    %eax,%esi\n"
//     "lea    0xc9e(%rip),%rax       \n"
//     "mov    %rax,%rdi\n"
//     "mov    $0x0,%eax\n"
//     "call   printf\n"
//     "mov    -0x4(%rbp),%edx\n"
//     "mov    -0x8(%rbp),%eax\n"
//     "add    %edx,%eax\n"
//     "leave\n"
//     "ret");

int roundup(int x, int y) { return (((x) + (y - 1)) / y) * y; }

size_t bpf_map_mmap_sz(unsigned int value_sz, unsigned int max_entries) {
  const long page_sz = sysconf(_SC_PAGE_SIZE);
  size_t map_sz;

  map_sz = (size_t)roundup(value_sz, 8) * max_entries;
  map_sz = roundup(map_sz, page_sz);
  return map_sz;
}

int main() {
  int a = 1;
  int b = 2;
  // can not directly call the function
  int c = foo1(a, b);
  printf("%d\n", c);

  // while (a < 1) {
  //   foo1(foo1(0, 1), foo1(3, 4));
  //   sleep(1);
  //   a++;
  // }
  // int (*func_ptr)(int a, int b) = foo1;
  // func_ptr(a, b);

  // indirect call is not allowed
  // func_ptr =
  //     (int (*)(int a, int b))((char *)func_ptr + 4); // go to ebpf adddress
  // printf("func_ptr = %p, original ptr = %p\n", func_ptr, foo1);
  // func_ptr(a, b);
  // couldn't probe into the uprobe

  // map the bss map to the user space with private and mprotect
  int kernel_map_id = 0;
  scanf("%d", &kernel_map_id);
  int map_fd = bpf_map_get_fd_by_id(kernel_map_id);
  if (map_fd < 0) {
    fprintf(stderr, "Failed to get fd for kernel map id %d\n", kernel_map_id);
    return 1;
  }
  struct bpf_map_info info;
  memset(&info, 0, sizeof(info));
  unsigned int info_len = sizeof(info);
  int res = bpf_obj_get_info_by_fd(map_fd, &info, &info_len);
  if (res < 0) {
    fprintf(stderr, "Failed to get info for kernel map id %d\n", kernel_map_id);
  }
  int _value_size = info.value_size;
  int _max_entries = info.max_entries;
  fprintf(stderr,
          "create kernel user array map value size %d, max entries %d\n",
          _value_size, _max_entries);
  size_t mmap_sz = bpf_map_mmap_sz(_value_size, _max_entries);
  fprintf(stderr,
          "mmap shared array map, fd=%d, mmap_sz=%ld, name=%s, value_size=%d, "
          "flags=%d\n",
          map_fd, mmap_sz, info.name, info.value_size, info.map_flags);
  void *mmap_ptr = mmap(NULL, mmap_sz, PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (mmap_ptr == MAP_FAILED) {
    fprintf(stderr, "Failed to mmap for kernel map id %d, err=%d\n",
            kernel_map_id, errno);
    return 1;
  }
  int pkey = pkey_alloc(0, PKEY_DISABLE_ACCESS);
  if (pkey == -1) {
    perror("pkey_alloc");
    return 1;
  }
  size_t pagesize = sysconf(_SC_PAGESIZE);
  if (pkey_mprotect(mmap_ptr, _value_size * _max_entries, PROT_READ, pkey) ==
      -1) {
    perror("pkey_mprotect");
    printf("%d\n", errno);
    return 1;
  }

  pkey_set(pkey, PKEY_DISABLE_ACCESS); // Disable access
                                       // Attempt to access the protected memory

  // access the function pointer of uprobe func
  int (*func1_ptr)(struct fake_pt_regs *, int size);

  uintptr_t func1_ptr_id;
  void *ptr;
  scanf("%" SCNxPTR, &func1_ptr_id);
  ptr = (void *)func1_ptr_id;
  printf("func1_ptr_id = %ld\n", func1_ptr_id);
  func1_ptr = (int (*)(struct fake_pt_regs *, int size))func1_ptr_id;
  // try to directly call the uprobe function pointer
  struct fake_pt_regs regs = {0, 0};
  func1_ptr(&regs, 2);

  func1_ptr = (int (*)(struct fake_pt_regs *, int size))(
      (char *)func1_ptr); // jmp to ebpf adddress
  // printf("func_ptr = %p, original ptr = %p\n", func1_ptr, foo1);
  func1_ptr(&regs, 2);

  printf("Attempting to access protected memory:%p\n", ptr);
  int val = *(
      (int *)
          mmap_ptr); // This will trigger a segmentation fault protected by mpk
  printf("val = %d\n", val);
  fflush(stdout);

  pkey_set(pkey, 0); // Enable access

  pkey_free(pkey);
  return 0;
}
