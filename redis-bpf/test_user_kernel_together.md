# test user and kernel

We only change fsync to iouring.

After submitting the fsync, when the next fsync come, we will check the previous fsync is finished or not.

We can use iouring to wait and check the previous fsync is finished or not, code like(redis-bpf/bpf-sync-next):

```c
SEC("uprobe/fsync")
int BPF_UPROBE(start_fsync, int __fd) {
  io_uring_wait_and_seen();
  io_uring_submit_fsync(__fd);
  io_uring_submit();
  bpf_override_return(0, 0);
  return 0;
}
```

we can also use kernel and user work together to achieve this, code like(redis-bpf/bpf-sync-kernel):

```c
SEC("uprobe/fsync")
int BPF_UPROBE(start_fsync, int __fd) {
  if (successful_writeback_count == 0) {
    io_uring_wait_and_seen();
  }
  successful_writeback_count = 0;
  io_uring_submit_fsync(__fd);
  io_uring_submit();
  bpf_override_return(0, 0);
  return 0;
}

SEC("kretprobe/file_write_and_wait_range")
int BPF_KPROBE(file_write_and_wait_range, struct file *file, loff_t start,
               loff_t end) {
  int pid = (bpf_get_current_pid_tgid() >> 32);
  if (pid != current_pid) {
    return 0;
  }
  __sync_fetch_and_add(&successful_writeback_count, 1);
  return 0;
}
```

instead of wait and check cqe, we use kernel to check the previous fsync is finished or not.

## result: no attach

```console
====== SET ======                                                   
  1000000 requests completed in 65.35 seconds
  5 parallel clients
  3 bytes payload
  keep alive: 1
  host configuration "save": 3600 1 300 100 60 10000
  host configuration "appendonly": yes
  multi-thread: no

Latency by percentile distribution:
0.000% <= 0.079 milliseconds (cumulative count 12)
50.000% <= 0.231 milliseconds (cumulative count 508445)
75.000% <= 0.335 milliseconds (cumulative count 750472)
87.500% <= 0.551 milliseconds (cumulative count 878886)
93.750% <= 0.703 milliseconds (cumulative count 938195)
96.875% <= 0.903 milliseconds (cumulative count 968978)
98.438% <= 0.999 milliseconds (cumulative count 984743)
99.219% <= 1.111 milliseconds (cumulative count 992322)
99.609% <= 1.223 milliseconds (cumulative count 996108)
99.805% <= 1.319 milliseconds (cumulative count 998089)
99.902% <= 1.423 milliseconds (cumulative count 999032)
99.951% <= 1.655 milliseconds (cumulative count 999514)
99.976% <= 1.935 milliseconds (cumulative count 999762)
99.988% <= 2.167 milliseconds (cumulative count 999878)
99.994% <= 2.415 milliseconds (cumulative count 999939)
99.997% <= 3.247 milliseconds (cumulative count 999970)
99.998% <= 6.535 milliseconds (cumulative count 999985)
99.999% <= 7.575 milliseconds (cumulative count 999993)
100.000% <= 8.783 milliseconds (cumulative count 999997)
100.000% <= 8.903 milliseconds (cumulative count 999999)
100.000% <= 8.927 milliseconds (cumulative count 1000000)
100.000% <= 8.927 milliseconds (cumulative count 1000000)

Cumulative distribution of latencies:
0.068% <= 0.103 milliseconds (cumulative count 678)
35.467% <= 0.207 milliseconds (cumulative count 354669)
72.716% <= 0.303 milliseconds (cumulative count 727162)
80.391% <= 0.407 milliseconds (cumulative count 803906)
85.282% <= 0.503 milliseconds (cumulative count 852822)
90.508% <= 0.607 milliseconds (cumulative count 905076)
93.820% <= 0.703 milliseconds (cumulative count 938195)
95.591% <= 0.807 milliseconds (cumulative count 955912)
96.898% <= 0.903 milliseconds (cumulative count 968978)
98.573% <= 1.007 milliseconds (cumulative count 985729)
99.199% <= 1.103 milliseconds (cumulative count 991990)
99.562% <= 1.207 milliseconds (cumulative count 995622)
99.787% <= 1.303 milliseconds (cumulative count 997868)
99.896% <= 1.407 milliseconds (cumulative count 998961)
99.930% <= 1.503 milliseconds (cumulative count 999297)
99.946% <= 1.607 milliseconds (cumulative count 999461)
99.958% <= 1.703 milliseconds (cumulative count 999582)
99.970% <= 1.807 milliseconds (cumulative count 999698)
99.974% <= 1.903 milliseconds (cumulative count 999744)
99.980% <= 2.007 milliseconds (cumulative count 999800)
99.984% <= 2.103 milliseconds (cumulative count 999841)
99.996% <= 3.103 milliseconds (cumulative count 999965)
99.997% <= 4.103 milliseconds (cumulative count 999970)
99.998% <= 5.103 milliseconds (cumulative count 999980)
99.999% <= 7.103 milliseconds (cumulative count 999990)
99.999% <= 8.103 milliseconds (cumulative count 999995)
100.000% <= 9.103 milliseconds (cumulative count 1000000)

Summary:
  throughput summary: 15303.39 requests per second
  latency summary (msec):
          avg       min       p50       p95       p99       max
        0.309     0.072     0.231     0.767     1.063     8.927
```

## result: use iouring wait cqe

we can use bpftime to hotpatch the sync function, and use iouring to wait cqe.

```console
====== SET ======                                                   
  1000000 requests completed in 52.54 seconds
  5 parallel clients
  3 bytes payload
  keep alive: 1
  host configuration "save": 3600 1 300 100 60 10000
  host configuration "appendonly": yes
  multi-thread: no

Latency by percentile distribution:
0.000% <= 0.087 milliseconds (cumulative count 1)
50.000% <= 0.191 milliseconds (cumulative count 563011)
75.000% <= 0.231 milliseconds (cumulative count 758421)
87.500% <= 0.383 milliseconds (cumulative count 876227)
93.750% <= 0.567 milliseconds (cumulative count 938558)
96.875% <= 0.735 milliseconds (cumulative count 968965)
98.438% <= 0.903 milliseconds (cumulative count 984436)
99.219% <= 0.991 milliseconds (cumulative count 992413)
99.609% <= 1.103 milliseconds (cumulative count 996172)
99.805% <= 1.199 milliseconds (cumulative count 998105)
99.902% <= 1.295 milliseconds (cumulative count 999072)
99.951% <= 1.391 milliseconds (cumulative count 999531)
99.976% <= 1.519 milliseconds (cumulative count 999758)
99.988% <= 1.751 milliseconds (cumulative count 999880)
99.994% <= 1.975 milliseconds (cumulative count 999940)
99.997% <= 2.079 milliseconds (cumulative count 999971)
99.998% <= 2.191 milliseconds (cumulative count 999985)
99.999% <= 2.231 milliseconds (cumulative count 999993)
100.000% <= 2.335 milliseconds (cumulative count 999997)
100.000% <= 2.375 milliseconds (cumulative count 999999)
100.000% <= 2.399 milliseconds (cumulative count 1000000)
100.000% <= 2.399 milliseconds (cumulative count 1000000)

Cumulative distribution of latencies:
0.076% <= 0.103 milliseconds (cumulative count 757)
68.564% <= 0.207 milliseconds (cumulative count 685641)
81.778% <= 0.303 milliseconds (cumulative count 817779)
88.633% <= 0.407 milliseconds (cumulative count 886328)
91.771% <= 0.503 milliseconds (cumulative count 917712)
94.817% <= 0.607 milliseconds (cumulative count 948169)
96.568% <= 0.703 milliseconds (cumulative count 965676)
97.576% <= 0.807 milliseconds (cumulative count 975763)
98.444% <= 0.903 milliseconds (cumulative count 984436)
99.315% <= 1.007 milliseconds (cumulative count 993150)
99.617% <= 1.103 milliseconds (cumulative count 996172)
99.821% <= 1.207 milliseconds (cumulative count 998209)
99.913% <= 1.303 milliseconds (cumulative count 999133)
99.958% <= 1.407 milliseconds (cumulative count 999577)
99.975% <= 1.503 milliseconds (cumulative count 999745)
99.982% <= 1.607 milliseconds (cumulative count 999821)
99.986% <= 1.703 milliseconds (cumulative count 999861)
99.989% <= 1.807 milliseconds (cumulative count 999894)
99.992% <= 1.903 milliseconds (cumulative count 999918)
99.995% <= 2.007 milliseconds (cumulative count 999945)
99.997% <= 2.103 milliseconds (cumulative count 999973)
100.000% <= 3.103 milliseconds (cumulative count 1000000)

Summary:
  throughput summary: 19031.67 requests per second
  latency summary (msec):
          avg       min       p50       p95       p99       max
        0.249     0.080     0.191     0.615     0.959     2.399

```

## result: using iouring wait cqe

start patch ebpf:

```console
root@banana:~/zys/bpftime-evaluation# SPDLOG_LEVEL=Debug BPFTIME_NOT_LOAD_PATTERN=start_.* BPFTIME_RUN_WITH_KERNEL=true LD_PRELOAD=/root/zys/bpftime/build/runtime/syscall-server/libbpftime-syscall-server.so /root/zys/bpftime-evaluation/redis-bpf/bpf-sync-next/rocksdb  
```

start redis:

```console
LD_PRELOAD=/root/zys/bpftime/build/runtime/agent/libbpftime-agent.so ./redis-server ../redis.conf
```

result

```console
====== SET ======                                                   
  1000000 requests completed in 25.20 seconds
  5 parallel clients
  3 bytes payload
  keep alive: 1
  host configuration "save": 3600 1 300 100 60 10000
  host configuration "appendonly": yes
  multi-thread: no

Latency by percentile distribution:
0.000% <= 0.023 milliseconds (cumulative count 5)
50.000% <= 0.111 milliseconds (cumulative count 573474)
75.000% <= 0.127 milliseconds (cumulative count 797575)
87.500% <= 0.143 milliseconds (cumulative count 907480)
93.750% <= 0.167 milliseconds (cumulative count 944503)
96.875% <= 0.231 milliseconds (cumulative count 970662)
98.438% <= 0.263 milliseconds (cumulative count 987533)
99.219% <= 0.287 milliseconds (cumulative count 993139)
99.609% <= 0.327 milliseconds (cumulative count 996214)
99.805% <= 0.399 milliseconds (cumulative count 998093)
99.902% <= 0.487 milliseconds (cumulative count 999035)
99.951% <= 0.663 milliseconds (cumulative count 999516)
99.976% <= 1.135 milliseconds (cumulative count 999758)
99.988% <= 1.183 milliseconds (cumulative count 999884)
99.994% <= 1.311 milliseconds (cumulative count 999939)
99.997% <= 2.463 milliseconds (cumulative count 999970)
99.998% <= 2.775 milliseconds (cumulative count 999985)
99.999% <= 3.111 milliseconds (cumulative count 999993)
100.000% <= 4.111 milliseconds (cumulative count 999997)
100.000% <= 4.247 milliseconds (cumulative count 999999)
100.000% <= 4.279 milliseconds (cumulative count 1000000)
100.000% <= 4.279 milliseconds (cumulative count 1000000)

Cumulative distribution of latencies:
44.173% <= 0.103 milliseconds (cumulative count 441730)
96.033% <= 0.207 milliseconds (cumulative count 960326)
99.480% <= 0.303 milliseconds (cumulative count 994802)
99.820% <= 0.407 milliseconds (cumulative count 998201)
99.916% <= 0.503 milliseconds (cumulative count 999155)
99.944% <= 0.607 milliseconds (cumulative count 999440)
99.957% <= 0.703 milliseconds (cumulative count 999566)
99.966% <= 0.807 milliseconds (cumulative count 999664)
99.969% <= 0.903 milliseconds (cumulative count 999689)
99.971% <= 1.007 milliseconds (cumulative count 999708)
99.972% <= 1.103 milliseconds (cumulative count 999721)
99.991% <= 1.207 milliseconds (cumulative count 999912)
99.994% <= 1.303 milliseconds (cumulative count 999938)
99.996% <= 1.407 milliseconds (cumulative count 999958)
99.996% <= 1.503 milliseconds (cumulative count 999960)
99.999% <= 3.103 milliseconds (cumulative count 999992)
100.000% <= 4.103 milliseconds (cumulative count 999996)
100.000% <= 5.103 milliseconds (cumulative count 1000000)

Summary:
  throughput summary: 39677.82 requests per second
  latency summary (msec):
          avg       min       p50       p95       p99       max
        0.114     0.016     0.111     0.183     0.271     4.279
```

## result: using kernel and user together

start patch ebpf:

```console
root@banana:~/zys/bpftime-evaluation# SPDLOG_LEVEL=Debug BPFTIME_NOT_LOAD_PATTERN=start_.* BPFTIME_RUN_WITH_KERNEL=true LD_PRELOA
D=/root/zys/bpftime/build/runtime/syscall-server/libbpftime-syscall-server.so /root/zys/bpftime-evaluation/redis-bpf/bpf-sync-kernel/rocksdb
```

start redis:

```console
LD_PRELOAD=/root/zys/bpftime/build/runtime/agent/libbpftime-agent.so ./redis-server ../redis.conf
```

result:

```console
====== SET ======                                                   
  1000000 requests completed in 15.45 seconds
  5 parallel clients
  3 bytes payload
  keep alive: 1
  host configuration "save": 3600 1 300 100 60 10000
  host configuration "appendonly": yes
  multi-thread: no

Latency by percentile distribution:
0.000% <= 0.023 milliseconds (cumulative count 135)
50.000% <= 0.063 milliseconds (cumulative count 565661)
75.000% <= 0.079 milliseconds (cumulative count 787207)
87.500% <= 0.095 milliseconds (cumulative count 900703)
93.750% <= 0.111 milliseconds (cumulative count 954202)
96.875% <= 0.119 milliseconds (cumulative count 970058)
98.438% <= 0.143 milliseconds (cumulative count 985113)
99.219% <= 0.183 milliseconds (cumulative count 992846)
99.609% <= 0.215 milliseconds (cumulative count 996114)
99.805% <= 0.287 milliseconds (cumulative count 998089)
99.902% <= 0.359 milliseconds (cumulative count 999077)
99.951% <= 0.439 milliseconds (cumulative count 999512)
99.976% <= 0.743 milliseconds (cumulative count 999761)
99.988% <= 1.167 milliseconds (cumulative count 999882)
99.994% <= 1.199 milliseconds (cumulative count 999949)
99.997% <= 1.271 milliseconds (cumulative count 999970)
99.998% <= 1.335 milliseconds (cumulative count 999985)
99.999% <= 1.391 milliseconds (cumulative count 999993)
100.000% <= 1.599 milliseconds (cumulative count 999997)
100.000% <= 1.607 milliseconds (cumulative count 999999)
100.000% <= 1.615 milliseconds (cumulative count 1000000)
100.000% <= 1.615 milliseconds (cumulative count 1000000)

Cumulative distribution of latencies:
93.711% <= 0.103 milliseconds (cumulative count 937109)
99.557% <= 0.207 milliseconds (cumulative count 995570)
99.836% <= 0.303 milliseconds (cumulative count 998364)
99.936% <= 0.407 milliseconds (cumulative count 999357)
99.963% <= 0.503 milliseconds (cumulative count 999631)
99.970% <= 0.607 milliseconds (cumulative count 999701)
99.974% <= 0.703 milliseconds (cumulative count 999742)
99.979% <= 0.807 milliseconds (cumulative count 999794)
99.980% <= 0.903 milliseconds (cumulative count 999803)
99.982% <= 1.007 milliseconds (cumulative count 999821)
99.983% <= 1.103 milliseconds (cumulative count 999827)
99.996% <= 1.207 milliseconds (cumulative count 999959)
99.998% <= 1.303 milliseconds (cumulative count 999977)
99.999% <= 1.407 milliseconds (cumulative count 999993)
99.999% <= 1.503 milliseconds (cumulative count 999994)
100.000% <= 1.607 milliseconds (cumulative count 999999)
100.000% <= 1.703 milliseconds (cumulative count 1000000)

Summary:
  throughput summary: 64724.92 requests per second
  latency summary (msec):
          avg       min       p50       p95       p99       max
        0.066     0.016     0.063     0.111     0.167     1.615

root@banana:~/zys/bpftime-evaluation/redis/src# 
```
