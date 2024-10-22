#ifndef HOOK_H
#define HOOK_H

void* get_function_addr_elf_pie(const char* func_name, char* err_msg);
void inline_hook(void *orig_func, void *hook_func);
void remove_hook(void *orig_func);

void my_hook_function();

#endif
