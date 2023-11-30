#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

int foo1(int a, int b) {
  printf("%d, %d\n", a, b);
  return a + b;
}
extern struct gate_bpf *skel;
int main() {
  int a = 1;
  int b = 2;
  int c = foo1(a, b);
  printf("%d\n", c);
  
  while (a<10) {
    foo1(foo1(0,1),foo1(3, 4));
    sleep(1);
    a++;
  }
  int (*func_ptr)(int a, int b) = foo1;
  func_ptr(a, b);

  func_ptr =
      (int (*)(int a, int b))((char *)func_ptr + 4); // go to ebpf adddress
  printf("func_ptr = %p, original ptr = %p\n", func_ptr, foo1);
  func_ptr(a, b);

  // access the map
  void *ptr= ((void *)0x55eeb49cbc80);
  printf("Attempting to access protected memory:\n");
  fflush(stdout);
  
  int val =
      *((int *)ptr); // This will trigger a segmentation fault protected by mpk

  return 0;
}