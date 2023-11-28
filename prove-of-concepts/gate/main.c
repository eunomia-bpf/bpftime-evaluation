#define _GNU_SOURCE
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>

int foo(int a, int b) {
  printf("%d, %d\n", a, b);
  return a + b;
}
extern struct gate_bpf *skel;
int main() {
  int pkey = pkey_alloc(0, PKEY_DISABLE_ACCESS);
  if (pkey == -1) {
    perror("pkey_alloc");
    return 1;
  }

  size_t pagesize = sysconf(_SC_PAGESIZE);
  void *ptr;
//   void *func_ptr;
  scanf ("%p",ptr);
//   scanf ("%p",ebpf_ptr);

  if (pkey_mprotect(ptr, pagesize, PROT_READ | PROT_WRITE, pkey) == -1) {
    perror("pkey_mprotect");
    munmap(ptr, pagesize);
    return 1;
  }

  pkey_set(pkey, PKEY_DISABLE_ACCESS); // Disable access
  // Attempt to access the protected memory
  printf("Attempting to access protected memory:\n");
  fflush(stdout);

  int a = 1;
  int b = 2;
  int c = foo(a, b);
  printf("%d\n", c);

  int (*func_ptr)(int a, int b) = foo;
  func_ptr(a, b);

  func_ptr =
      (int (*)(int a, int b))((char *)func_ptr + 4); // go to ebpf adddress
  printf("func_ptr = %p, original ptr = %p\n", func_ptr, foo);
  func_ptr(a, b);

  // access the map
  printf("Attempting to access protected memory:\n");
  fflush(stdout);
  int val = *((int *)ptr); // This will trigger a segmentation fault

  pkey_set(pkey, 0); // Enable access
  printf("Access enabled. Value at ptr: %d\n", val);

  munmap(ptr, pagesize);
  pkey_free(pkey);

  return 0;
}