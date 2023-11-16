#ifndef _FUNC_DEF_H
#define _FUNC_DEF_H
#include <inttypes.h>

typedef int64_t (*syscall_hooker_func_t)(int64_t sys_nr, int64_t arg1,
					 int64_t arg2, int64_t arg3,
					 int64_t arg4, int64_t arg5,
					 int64_t arg6);
#endif
