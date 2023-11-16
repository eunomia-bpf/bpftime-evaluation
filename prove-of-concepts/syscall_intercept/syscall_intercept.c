#define _GNU_SOURCE
#include <dlfcn.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <stdarg.h>

typedef long (*orig_syscall_type)(long number, ...);

long syscall(long number, ...) {
    orig_syscall_type orig_syscall;
    orig_syscall = (orig_syscall_type)dlsym(RTLD_NEXT, "syscall");
    
    // Print the intercepted syscall number
    printf("Intercepted syscall: %ld\n", number);
    fflush(stdout);
    
    // Call the original syscall function with its arguments
    va_list args;
    va_start(args, number);
    long ret = orig_syscall(number, args);
    va_end(args);
    return ret;
}

int open(const char *pathname, int flags, ...) {
    static int (*real_open)(const char *, int, ...) = NULL;

    if (!real_open) {
        real_open = dlsym(RTLD_NEXT, "open");
    }

    printf("open(\"%s\", %d)\n", pathname, flags);
    va_list args;
    va_start(args, flags);
    long ret = real_open(pathname, flags, args);
    va_end(args);
    return ret;
}