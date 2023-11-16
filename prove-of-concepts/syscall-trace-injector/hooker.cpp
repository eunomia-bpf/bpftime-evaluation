#include <cinttypes>
#include "def.h"
#include <iostream>
#include <ostream>
syscall_hooker_func_t orig_syscall;
extern "C" int64_t hooker_func(int64_t sys_nr, int64_t arg1, int64_t arg2,
			       int64_t arg3, int64_t arg4, int64_t arg5,
			       int64_t arg6)
{
	std::cout << "Syscall " << sys_nr << ": " << arg1 << " " << arg2 << " "
		  << arg3 << " " << arg4 << " " << arg5 << " " << arg6
		  << std::endl;
	return orig_syscall(sys_nr, arg1, arg2, arg3, arg4, arg5, arg6);
}
extern "C" void setup_hooker(syscall_hooker_func_t *hooker)
{
	orig_syscall = *hooker;
	*hooker = hooker_func;
}
