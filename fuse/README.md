# test fuse

## How to?

cache or filter

userspace cache, but invalid in kernel

## workload

Test for linux source tree compression/decompression

The syscall of linux compression/decompression:

```console
$ strace -c time tar czf linux.tar.gz linux/
135.20user 155.30system 4:02.60elapsed 119%CPU (0avgtext+0avgdata 2944maxresident)k
5985960inputs+7846976outputs (2major+584minor)pagefaults 0swaps
% time     seconds  usecs/call     calls    errors syscall
------ ----------- ----------- --------- --------- ----------------
100.00  155.314474   155314474         1           wait4
  0.00    0.003373          30       112           write
  0.00    0.000018           4         4           rt_sigaction
  0.00    0.000000           0         1           read
  0.00    0.000000           0         2           close
  0.00    0.000000           0         8           mmap
  0.00    0.000000           0         3           mprotect
  0.00    0.000000           0         1           munmap
  0.00    0.000000           0         1           brk
  0.00    0.000000           0         2           pread64
  0.00    0.000000           0         1         1 access
  0.00    0.000000           0         1           clone
  0.00    0.000000           0         1           execve
  0.00    0.000000           0         2         1 arch_prctl
  0.00    0.000000           0         1           set_tid_address
  0.00    0.000000           0         2           openat
  0.00    0.000000           0         2           newfstatat
  0.00    0.000000           0         1           set_robust_list
  0.00    0.000000           0         1           prlimit64
  0.00    0.000000           0         1           rseq
------ ----------- ----------- --------- --------- ----------------
100.00  155.317865     1049445       148         2 total
(venv) yunwei@yunwei37server:~$ 
```

The syscall of linux find:

```console
$ strace -c find linux/ > /dev/null 
% time     seconds  usecs/call     calls    errors syscall
------ ----------- ----------- --------- --------- ----------------
 33.07    0.275390          12     21704           close
 27.10    0.225694           8     27051           fcntl
 18.09    0.150641           9     16295           newfstatat
 15.50    0.129091          11     10824           getdents64
  5.19    0.043255           7      5485        12 openat
  0.89    0.007372           7      1038           write
  0.05    0.000398          22        18           mmap
  0.03    0.000284          23        12           brk
  0.03    0.000244         244         1           execve
  0.01    0.000119          17         7           read
  0.01    0.000088          17         5           mprotect
  0.01    0.000048          16         3         2 ioctl
  0.00    0.000036          18         2         2 statfs
  0.00    0.000035          17         2         2 access
  0.00    0.000028          28         1           munmap
  0.00    0.000026          13         2           pread64
  0.00    0.000020          10         2         1 arch_prctl
  0.00    0.000015          15         1           fstatfs
  0.00    0.000014          14         1           prlimit64
  0.00    0.000013          13         1           uname
  0.00    0.000013          13         1           getrandom
  0.00    0.000012          12         1           set_tid_address
  0.00    0.000012          12         1           set_robust_list
  0.00    0.000012          12         1           rseq
  0.00    0.000010          10         1           fchdir
------ ----------- ----------- --------- --------- ----------------
100.00    0.832870          10     82460        19 total
```
