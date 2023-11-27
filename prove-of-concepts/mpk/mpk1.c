#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

// Declaration of pkey_set function
int pkey_set(int __key, unsigned int __access_rights);

// Function to generate machine code for modifying access
unsigned char* generate_access_function(void) {
    unsigned char* code = mmap(NULL, 4096, PROT_READ | PROT_WRITE | PROT_EXEC, 
                               MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (code == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // Assembly code to move the pkey value into the second argument (rdx)
    // and then call the pkey_set function.
    unsigned char machine_code[] = {
        0xBA, 0x00, 0x00, 0x00, 0x00,       // mov edx, {pkey} placeholder
        0xBE, 0x00, 0x00, 0x00, 0x00,       // mov esi, {access_rights} placeholder
        0x48, 0xB8,                         // mov rax, &pkey_set
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xFF, 0xD0,                         // call rax
        0xC3                                // ret
    };

    // Insert the address of the pkey_set function into the machine code
    void *pkey_set_addr = (void *)&pkey_set;
    memcpy(&machine_code[10], &pkey_set_addr, sizeof(pkey_set_addr));

    // Copy the machine code into the allocated memory
    memcpy(code, machine_code, sizeof(machine_code));

    return code;
}

int main() {
    int pkey = pkey_alloc(0, PKEY_DISABLE_ACCESS);

    unsigned char* modify_access = generate_access_function();

    // Now, modify_access points to JIT-generated code that expects the pkey and
    // access rights as its first and second arguments, respectively.
    void (*change_access)(int, unsigned int) = (void (*)(int, unsigned int))modify_access;

    change_access(pkey, PKEY_DISABLE_ACCESS); // Disable access
    // ... Code that should run with the memory access disabled ...
    change_access(pkey, 0);  // Enable access

    munmap(modify_access, 4096);
    pkey_free(pkey);

    return 0;
}
