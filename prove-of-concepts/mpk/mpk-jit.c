#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

// Declaration of pkey_set function
int pkey_set(int __key, unsigned int __access_rights);

unsigned char *generate_access_function(int pkey)
{
    unsigned char *code = mmap(NULL, 4096, PROT_READ | PROT_WRITE | PROT_EXEC,
                               MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (code == MAP_FAILED)
    {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // Machine code to move the pkey value into the edi register (for x86_64 ABI)
    unsigned char machine_code[] = {
        0x48, 0xC7, 0xC7, // mov rdi, imm32
        pkey & 0xFF, (pkey >> 8) & 0xFF, (pkey >> 16) & 0xFF, (pkey >> 24) & 0xFF,
        0x48, 0x89, 0xC6, // mov rsi, rax (move the function argument into esi)
        0x48, 0xB8,       // movabs rax, imm64 (address of pkey_set)
        // Placeholder for the address of pkey_set; will be filled in at runtime
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xFF, 0xD0, // call rax
        0xC3        // ret
    };

    // Insert the address of pkey_set into the machine code
    void *pkey_set_addr = (void *)&pkey_set;
    memcpy(&machine_code[12], &pkey_set_addr, sizeof(pkey_set_addr));

    // Copy the machine code into the allocated memory
    memcpy(code, machine_code, sizeof(machine_code));
    return code;
}

int main()
{
    int pkey = pkey_alloc(0, PKEY_DISABLE_ACCESS);

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

    unsigned char *modify_access = generate_access_function(pkey);

    void (*change_access)(unsigned long) = (void (*)(unsigned long))modify_access;

    change_access(PKEY_DISABLE_ACCESS); // Disable access
    // Attempt to access the protected memory
    printf("Attempting to access protected memory:\n");

    int val = *((int*)ptr); // This will trigger a segmentation fault

    change_access(0); // Enable access

    munmap(modify_access, 4096);
    
    pkey_free(pkey);

    return 0;
}
