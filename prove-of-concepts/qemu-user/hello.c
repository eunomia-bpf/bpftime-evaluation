#include <stdio.h>
#include <assert.h>
#include <stdint.h>

int data = 42;

int func_i(int i)
{
	assert(i == 35);
	return i;
}

int func_i_p(int i, int *p)
{
	assert(i == 35);
	assert(*p == 42);
	return i + *p;
}

uint64_t func_i_p_z_q(int i, int *p, char z, uint64_t q)
{
	assert(i == 35);
	assert(*p == 42);
	assert(z == 'z');
	assert(q == 0xdeadbeef);
	return i + *p;
}

typedef void *(*ffi_function_t)(uintptr_t arg1, uintptr_t arg2, uintptr_t arg3,
				uintptr_t arg4);

void test_ffi_call()
{
	int i = 35;
	int *p = &data;
	char z = 'z';
	uint64_t q = 0xdeadbeef;

	// test normal function call
	int res = func_i(i);
	assert(res == i);
	res = func_i_p(i, p);
	assert(res == i + *p);
	res = func_i_p_z_q(i, p, z, q);
	assert(res == i + *p);

	// test ffi call
    ffi_function_t ffi_func_i = (ffi_function_t)func_i;
    res = (int)ffi_func_i(i, 0, 0, 0);
    assert(res == i);
    ffi_function_t ffi_func_i_p = (ffi_function_t)func_i_p;
    res = (int)ffi_func_i_p(i, (uintptr_t)p, 0, 0);
    assert(res == i + *p);
    ffi_function_t ffi_func_i_p_z_q = (ffi_function_t)func_i_p_z_q;
    res = (int)ffi_func_i_p_z_q(i, (uintptr_t)p, (uintptr_t)z, (uintptr_t)q);
    assert(res == i + *p);
}

int main()
{
	printf("Hello, World!\n");
    test_ffi_call();
	return 0;
}
