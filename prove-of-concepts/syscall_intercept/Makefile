syscall_intercept: syscall_intercept.c
	gcc -shared -fPIC syscall_intercept.c -o syscall_intercept.so -ldl

test: test.c
	gcc test.c -o test

run: syscall_intercept test
	LD_PRELOAD=./syscall_intercept.so ./test