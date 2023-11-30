#include <stdio.h>
#include <stdlib.h>

void safe_function() {
    printf("Safe function called.\n");
}

void unsafe_function() {
    printf("Unsafe function called.\n");
}

int main() {
    // 正常的函数指针使用
    // void (*func_ptr)() = safe_function;
    // func_ptr();
    safe_function();

    // 尝试非法修改函数指针
    // 在一个CET支持的环境中，这种修改会被检测并阻止
    printf("sb");
    void (*func_ptr)() = safe_function;

    // printf("func_ptr = %p, original ptr = %p\n", func_ptr, unsafe_function);
    func_ptr = (void (*)())((char*)unsafe_function+4);
    func_ptr(); // 这里的调用在CET环境下应该会失败

    return 0;
}


// #include <stdint.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <sys/types.h>
// #include <unistd.h>
// void func1(void) { printf("[+] Success: Inside func1\n"); }
// void func2(void) { printf("[+] Success: Inside func2\n"); }
// int main(void)
// {
//   void (*fptr)(void);
//   pid_t (*getpid_fptr)(void);
//   pid_t pid;
//   printf("[o] Indirectly calling external function: getpid()\n");
//   getpid_fptr = &getpid;
//   pid = getpid_fptr();
//   printf("[+] Success: PID = %jd\n", (intmax_t)pid);
//   printf("[o] Directly calling function: func1()\n");
//   func1();
//   printf("[o] Indirectly calling function: func2()\n");
//   func2();
//   printf("[+] Done.\n");
// }
