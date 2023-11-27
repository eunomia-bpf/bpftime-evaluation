all: cet_demo no_cet_demo
cet_demo: cet_demo.c
	gcc -fcf-protection=full -o cet_demo cet_demo.c

no_cet_demo: cet_demo.c
	gcc -o no_cet_demo cet_demo.c
