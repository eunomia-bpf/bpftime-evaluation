all: cet_demo.s no_cet_demo test
cet_demo: cet_demo.c
	clang -fsanitize=shadow-call-stack -O2 -o cet_demo cet_demo.c

no_cet_demo: cet_demo.c
	gcc -o no_cet_demo -O2 cet_demo.c

cet_demo.s: cet_demo
	objdump -d cet_demo > cet_demo.s

test: test.c
	gcc -o test -O2 test.c

clean: 
	rm -f cet_demo no_cet_demo test cet_demo.s