#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

int main() {
    int pkey = pkey_alloc(0, PKEY_DISABLE_ACCESS);
    if (pkey == -1) {
        perror("pkey_alloc");
        return 1;
    }

    size_t pagesize = sysconf(_SC_PAGESIZE);
    void *ptr = mmap(NULL, pagesize, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    if (pkey_mprotect(ptr, pagesize, PROT_READ | PROT_WRITE, pkey) == -1) {
        perror("pkey_mprotect");
        munmap(ptr, pagesize);
        return 1;
    }

    pkey_set(pkey, PKEY_DISABLE_ACCESS); // Disable access
    // Attempt to access the protected memory
    printf("Attempting to access protected memory:\n");
    fflush(stdout);
    int val = *((int*)ptr); // This will trigger a segmentation fault

    pkey_set(pkey, 0); // Enable access
    printf("Access enabled. Value at ptr: %d\n", val);

    munmap(ptr, pagesize);
    pkey_free(pkey);
    return 0;
}