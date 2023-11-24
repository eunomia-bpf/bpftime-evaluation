# fs-cache

- cache syscall metadata
- block access to certain files

## cache syscall metadata

## Usage

run agent

```console
# AGENT_SO=build/runtime/agent/libbpftime-agent.so LD_PRELOAD=build/runtime/agent-transformer/libbpftime-agent-transformer.so find ./example
```

run server:

```console
sudo LD_PRELOAD=build/runtime/syscall-server/libbpftime-syscall-server.so example/fs-filter-cache/fs-cache
```

## cache for getdents64

base line of getdents64 syscall

- Tested with Passthrough fuse:

start fuse:

```console
source ~/OpenCopilot/venv/bin/activate 
python3 passthrough.py  ~/linux ./data/
```

```console
root@yunwei37server:/home/yunwei/bpftime# /home/yunwei/bpftime/example/fs-filter-cache/bench getdents64 /home/yunwei/bpftime-evaluation/fuse/data/virt
inode=4294967295 offset=32 reclen=24 type=0 name=.
inode=4294967295 offset=64 reclen=24 type=0 name=..
inode=4294967295 offset=96 reclen=24 type=0 name=kvm
inode=4294967295 offset=128 reclen=24 type=0 name=lib
inode=4294967295 offset=160 reclen=32 type=0 name=Makefile
Average time usage 36526.149010 ns, iter 100000 times
```

use cache for getdents64 syscall

```console
# AGENT_SO=build/runtime/agent/libbpftime-agent.so LD_PRELOAD=build/runtime/agent-transformer/libbpftime-agent-transformer.so  /home/yunwei/bpftime/example/fs-filter-cache/bench getdents64  /home/yunwei/bpftime-evaluation/fuse/data/virt
....
inode=4294967295 offset=32 reclen=24 type=0 name=.
inode=4294967295 offset=64 reclen=24 type=0 name=..
inode=4294967295 offset=96 reclen=24 type=0 name=kvm
inode=4294967295 offset=128 reclen=24 type=0 name=lib
inode=4294967295 offset=160 reclen=32 type=0 name=Makefile
Average time usage 1761.278320 ns, iter 100000 times

[2023-11-23 23:38:52][info][414216] Compiling using LLJIT
INFO [414216]: Global shm destructed
```

Test with loggerFS:

```sh
sudo loggedfs -l ~/log.txt /home/yunwei/bpftime/daemon
```

No cache:

```console
root@yunwei37server:/home/yunwei/bpftime# /home/yunwei/bpftime/example/fs-filter-cache/bench getdents64 /home/yunwei/bpftime/daemon
inode=1710896 offset=40 reclen=32 type=8 name=README.md
inode=1761315 offset=72 reclen=24 type=4 name=user
inode=1753980 offset=104 reclen=24 type=4 name=.
inode=1710893 offset=144 reclen=32 type=8 name=.gitignore
inode=1717723 offset=176 reclen=24 type=4 name=..
inode=1754009 offset=208 reclen=32 type=4 name=kernel
inode=1761304 offset=240 reclen=24 type=4 name=test
inode=1710897 offset=288 reclen=40 type=8 name=bpf_tracer_event.h
inode=1710894 offset=328 reclen=40 type=8 name=CMakeLists.txt
Average time usage 74006.800000 ns, iter 100 times
```

cache:

```console
root@yunwei37server:/home/yunwei/bpftime# AGENT_SO=build/runtime/agent/libbpftime-agent.so LD_PRELOAD=build/runtime/agent-transformer/libbpftime-agent-transformer.so  /home/yunwei/bpftime/example/fs-filter-cache/bench getdents64 /home/yunwei/bpftime/daemon
...
inode=1710896 offset=40 reclen=32 type=8 name=README.md
inode=1761315 offset=72 reclen=24 type=4 name=user
inode=1753980 offset=104 reclen=24 type=4 name=.
inode=1710893 offset=144 reclen=32 type=8 name=.gitignore
inode=1717723 offset=176 reclen=24 type=4 name=..
inode=1754009 offset=208 reclen=32 type=4 name=kernel
inode=1761304 offset=240 reclen=24 type=4 name=test
inode=1710897 offset=288 reclen=40 type=8 name=bpf_tracer_event.h
inode=1710894 offset=328 reclen=40 type=8 name=CMakeLists.txt
Average time usage 1837.819106 ns, iter 1000000 times
```
