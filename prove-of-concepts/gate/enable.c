#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>  

int main() {
  uint64_t *addr = malloc(sizeof(uint64_t) * 4);
  arch_prctl(0x3001, addr);
  printf("[+] Success: addr = %p\n", addr[0]);
  printf("[+] Success: addr = %p\n", addr[1]);
  printf("[+] Success: addr = %p\n", addr[2]);
}