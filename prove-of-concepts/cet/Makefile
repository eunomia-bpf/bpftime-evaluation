all: no_cet_demo.s cet_demo.s test
cet_demo: cet_demo.c
	#musl-clang -O2 -flto -fcf-protection=full -Wl,-z,relro,-z,now,-z,force-ibt,-z,force-fine-ibt -o cet_demo cet_demo.c
	 gcc -mshstk -fcf-protection=full -O2 -o cet_demo cet_demo.c
no_cet_demo: cet_demo.c
	gcc -o no_cet_demo -O2 cet_demo.c

cet_demo.s: cet_demo
	objdump -d cet_demo > cet_demo.s


no_cet_demo.s: no_cet_demo
	objdump -d no_cet_demo > no_cet_demo.s

test: test.c
	gcc -o test -O2 test.c

clean: 
	rm -f cet_demo no_cet_demo test cet_demo.s no_cet_demo.s
