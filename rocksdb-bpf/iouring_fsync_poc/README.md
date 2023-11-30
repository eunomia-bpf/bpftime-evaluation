# use ebpf to speed up iouring with fsync



```console
root@banana:~/zys/bpftime-evaluation/rocksdb-bpf/iouring_fsync_poc# ./iouring_fsync 
Starting test
Completed operations: 91126
batch_size = 1 completed
Completed operations: 135130
batch_size = 2 completed
Completed operations: 311476
batch_size = 4 completed
Completed operations: 266584
batch_size = 8 completed
Completed operations: 269296
batch_size = 16 completed
Completed operations: 332512
batch_size = 32 completed
Completed operations: 319488
batch_size = 64 completed
Completed operations: 353024
batch_size = 128 completed
Completed operations: 396544
batch_size = 256 completed
Completed operations: 475136
batch_size = 512 completed
root@banana:~/zys/bpftime-evaluation/rocksdb-bpf/iouring_fsync_poc# make
  BPF      .output/iouring_fsync.bpf.o
  GEN-SKEL .output/iouring_fsync.skel.h
  CC       .output/iouring_fsync.o
  BINARY   iouring_fsync
root@banana:~/zys/bpftime-evaluation/rocksdb-bpf/iouring_fsync_poc# make
  CC       .output/iouring_fsync.o
  BINARY   iouring_fsync
root@banana:~/zys/bpftime-evaluation/rocksdb-bpf/iouring_fsync_poc# ./iouring_fsync 
Starting test
Completed operations: 768707
batch_size = 1 completed
Completed operations: 636912
batch_size = 2 completed
Completed operations: 796352
batch_size = 4 completed
Completed operations: 702360
batch_size = 8 completed
Completed operations: 763968
batch_size = 16 completed
Completed operations: 763168
batch_size = 32 completed
Completed operations: 784384
batch_size = 64 completed
Completed operations: 874496
batch_size = 128 completed
Completed operations: 828928
batch_size = 256 completed
Completed operations: 793088
batch_size = 512 completed
root@banana:~/zys/bpftime-evaluation/rocksdb-bpf/iouring_fsync_poc# 
```