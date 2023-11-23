.PHONY: all
all: getdents64 newfsstatat fs
getdents64: getdents64.c
	gcc getdents64.c -o ./getdents64
newfsstatat: newfsstatat.c
	gcc newfsstatat.c -o ./newfsstatat
fs: fs.cpp
	g++ fs.cpp -o ./fs