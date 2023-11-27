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
    void (*func_ptr)() = safe_function;
    func_ptr();

    // 尝试非法修改函数指针
    // 在一个CET支持的环境中，这种修改会被检测并阻止
    func_ptr = (void (*)())((char*)unsafe_function + 4);
    printf("func_ptr = %p, original ptr = %p\n", func_ptr, unsafe_function);
    func_ptr(); // 这里的调用在CET环境下应该会失败

    return 0;
}
