# test fuse

## How to?

cache or filter

userspace cache, but invalid in kernel

## workload

Test for linux source tree compression/decompression

The syscall of linux compression/decompression:

```console
$ strace -c -f time t
ar czf linux.tar.gz linux/
strace: Process 26014 attached
strace: Process 26015 attached
strace: Process 26016 attached
139.02user 241.99system 9:52.16elapsed 64%CPU (0avgtext+0avgdata 2944maxresident)k
11277648inputs+7845632outputs (13major+564minor)pagefaults 0swaps
% time     seconds  usecs/call     calls    errors syscall
------ ----------- ----------- --------- --------- ----------------
 81.03  476.171706    79361951         6         3 wait4
 11.31   66.494371          68    976564         1 read
  6.20   36.409685          64    567822           write
  0.85    5.005722          18    274889         6 newfstatat
  0.29    1.707477          19     89813        24 openat
  0.26    1.532994          17     89804           close
  0.03    0.192929          17     10824           getdents64
  0.03    0.147562          13     10825           fcntl
  0.00    0.001381          22        62           brk
  0.00    0.000910          14        65           readlinkat
  0.00    0.000450           9        46           mmap
  0.00    0.000150          10        15           mprotect
  0.00    0.000144          72         2           clone
  0.00    0.000086          21         4         4 connect
  0.00    0.000082          20         4           socket
  0.00    0.000064          10         6           lseek
  0.00    0.000045          11         4           munmap
  0.00    0.000040          20         2         2 statfs
  0.00    0.000035           1        26           rt_sigaction
  0.00    0.000026           3         8           pread64
  0.00    0.000021          21         1           pipe2
  0.00    0.000019           3         5         5 access
  0.00    0.000019           4         4           prlimit64
  0.00    0.000013           6         2           getrandom
  0.00    0.000012           1         8         4 arch_prctl
  0.00    0.000012           3         4           set_tid_address
  0.00    0.000012           3         4           rseq
  0.00    0.000011           1         6           set_robust_list
  0.00    0.000000           0         3           rt_sigprocmask
  0.00    0.000000           0         1           rt_sigreturn
  0.00    0.000000           0         1         1 ioctl
  0.00    0.000000           0         2           dup
  0.00    0.000000           0         1           getpid
  0.00    0.000000           0         1           vfork
  0.00    0.000000           0        10         6 execve
  0.00    0.000000           0         1           creat
  0.00    0.000000           0         1           getuid
  0.00    0.000000           0         1           getgid
  0.00    0.000000           0         2           geteuid
  0.00    0.000000           0         1           getegid
  0.00    0.000000           0         1           getppid
------ ----------- ----------- --------- --------- ----------------
100.00  587.665978         290   2020851        56 total
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
