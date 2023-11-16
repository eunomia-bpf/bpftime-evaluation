myapp:
	gcc main.c -o myapp

.PHONY: install uprobe
install:
	sudo apt-get install systemtap-sdt-dev

uprobe:
	sudo bpftrace -e 'usdt:./myapp:myprovider:myprobe { printf("%d %d\n", arg0, arg1); }