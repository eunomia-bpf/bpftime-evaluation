#include <stdio.h>
#include <cpuid.h>
#include <stdint.h>

int cpu_supports_cet_shadow_stack() {
    uint32_t eax = 0, ebx = 0, ecx = 0, edx = 0;
    __cpuid_count(7, 0, eax, ebx, ecx, edx);
    return (ecx & (1 << 7)) != 0;
}

int cpu_supports_cet_ibt() {
    uint32_t eax = 0, ebx = 0, ecx = 0, edx = 0;
    __cpuid_count(7, 0, eax, ebx, ecx, edx);
    return (edx & (1 << 20)) != 0;
}

int main() {
    if (cpu_supports_cet_shadow_stack()) {
        puts("CET Shadow Stack is supported");
    }

    if (cpu_supports_cet_ibt()) {
        puts("CET IBT is supported");
    }
}