#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <signal.h>

void handle_sigsegv(int sig) {
    printf("Caught segmentation fault (SIGSEGV).\n");
    exit(1);
}

int main() {
    int pkey;

    // 设置信号处理函数以捕获段错误
    signal(SIGSEGV, handle_sigsegv);

    // 分配一个保护密钥
    pkey = pkey_alloc(0, 0);
    if (pkey == -1) {
        perror("pkey_alloc");
        return 1;
    }

    // 分配一段内存
    size_t pagesize = sysconf(_SC_PAGESIZE);
    void* buffer = mmap(NULL, pagesize, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (buffer == MAP_FAILED) {
        perror("mmap");
        pkey_free(pkey);
        return 1;
    }

    // 使用密钥保护这段内存
    if (pkey_mprotect(buffer, pagesize, PROT_READ | PROT_WRITE, pkey) == -1) {
        perror("pkey_mprotect");
        munmap(buffer, pagesize);
        pkey_free(pkey);
        return 1;
    }

    // 使用内存
    *(int *)buffer = 123;
    printf("Value: %d\n", *(int *)buffer);

    // 更改内存区域的权限
    if (pkey_set(pkey, PROT_READ) == -1) {
        perror("pkey_set");
    }

    // 再次尝试写入内存
    *(int *)buffer = 456; // 这里可能会引发段错误

    // 清理
    munmap(buffer, pagesize);
    pkey_free(pkey);

    return 0;
}
