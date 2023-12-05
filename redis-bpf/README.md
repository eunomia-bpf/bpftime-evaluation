# redis AOF with bpf

## config

```conf
# The fsync() call tells the Operating System to actually write data on disk
# instead of waiting for more data in the output buffer. Some OS will really flush
# data on disk, some other OS will just try to do it ASAP.
#
# Redis supports three different modes:
#
# no: don't fsync, just let the OS flush the data when it wants. Faster.
# always: fsync after every write to the append only log. Slow, Safest.
# everysec: fsync only one time every second. Compromise.
#
# The default is "everysec", as that's usually the right compromise between
# speed and data safety. It's up to you to understand if you can relax this to
# "no" that will let the operating system flush the output buffer when
# it wants, for better performances (but if you can live with the idea of
# some data loss consider the default persistence mode that's snapshotting),
# or on the contrary, use "always" that's very slow but a bit safer than
# everysec.
#
# More details please check the following article:
# http://antirez.com/post/redis-persistence-demystified.html
#
# If unsure, use "everysec".

appendfsync always
# appendfsync everysec
# appendfsync no

# When the AOF fsync policy is set to always or everysec, and a background
# saving process (a background save or AOF log background rewriting) is
# performing a lot of I/O against the disk, in some Linux configurations
# Redis may block too long on the fsync() call. Note that there is no fix for
# this currently, as even performing fsync in a different thread will block
# our synchronous write(2) call.
#
# In order to mitigate this problem it's possible to use the following option
# that will prevent fsync() from being called in the main process while a
# BGSAVE or BGREWRITEAOF is in progress.
#
# This means that while another child is saving, the durability of Redis is
# the same as "appendfsync no". In practical terms, this means that it is
# possible to lose up to 30 seconds of log in the worst scenario (with the
# default Linux settings).
#
# If you have latency problems turn this to "yes". Otherwise leave it as
# "no" that is the safest pick from the point of view of durability.

no-appendfsync-on-rewrite no

# Automatic rewrite of the append only file.
# Redis is able to automatically rewrite the log file implicitly calling
# BGREWRITEAOF when the AOF log size grows by the specified percentage.
#
# This is how it works: Redis remembers the size of the AOF file after the
# latest rewrite (if no rewrite has happened since the restart, the size of
# the AOF at startup is used).
#
# This base size is compared to the current size. If the current size is
# bigger than the specified percentage, the rewrite is triggered. Also
# you need to specify a minimal size for the AOF file to be rewritten, this
# is useful to avoid rewriting the AOF file even if the percentage increase
# is reached but it is still pretty small.
#
# Specify a percentage of zero in order to disable the automatic AOF
# rewrite feature.

auto-aof-rewrite-percentage 0
auto-aof-rewrite-min-size 64mb
```

## test without AOF

```console
root@banana:~/zys/bpftime-evaluation/redis/src# redis-benchmark -t set -n 10000
00 -r 100000000 -c 5
====== SET ======                                                    
  1000000 requests completed in 11.44 seconds
  5 parallel clients
  3 bytes payload
  keep alive: 1
  host configuration "save": 3600 1 300 100 60 10000
  host configuration "appendonly": yes
  multi-thread: no

Latency by percentile distribution:
0.000% <= 0.015 milliseconds (cumulative count 4044)
50.000% <= 0.047 milliseconds (cumulative count 677019)
75.000% <= 0.055 milliseconds (cumulative count 807038)
87.500% <= 0.063 milliseconds (cumulative count 904447)
93.750% <= 0.071 milliseconds (cumulative count 940820)
96.875% <= 0.087 milliseconds (cumulative count 971256)
98.438% <= 0.103 milliseconds (cumulative count 986648)
99.219% <= 0.119 milliseconds (cumulative count 992715)
99.609% <= 0.159 milliseconds (cumulative count 996446)
99.805% <= 0.239 milliseconds (cumulative count 998074)
99.902% <= 0.351 milliseconds (cumulative count 999075)
99.951% <= 0.447 milliseconds (cumulative count 999520)
99.976% <= 0.711 milliseconds (cumulative count 999756)
99.988% <= 1.127 milliseconds (cumulative count 999898)
99.994% <= 1.167 milliseconds (cumulative count 999939)
99.997% <= 1.215 milliseconds (cumulative count 999972)
99.998% <= 1.263 milliseconds (cumulative count 999986)
99.999% <= 1.471 milliseconds (cumulative count 999993)
100.000% <= 1.575 milliseconds (cumulative count 999998)
100.000% <= 1.599 milliseconds (cumulative count 999999)
100.000% <= 1.623 milliseconds (cumulative count 1000000)
100.000% <= 1.623 milliseconds (cumulative count 1000000)

Cumulative distribution of latencies:
98.665% <= 0.103 milliseconds (cumulative count 986648)
99.766% <= 0.207 milliseconds (cumulative count 997663)
99.868% <= 0.303 milliseconds (cumulative count 998676)
99.939% <= 0.407 milliseconds (cumulative count 999386)
99.967% <= 0.503 milliseconds (cumulative count 999674)
99.973% <= 0.607 milliseconds (cumulative count 999731)
99.975% <= 0.703 milliseconds (cumulative count 999753)
99.978% <= 0.807 milliseconds (cumulative count 999781)
99.982% <= 0.903 milliseconds (cumulative count 999819)
99.982% <= 1.007 milliseconds (cumulative count 999821)
99.985% <= 1.103 milliseconds (cumulative count 999849)
99.997% <= 1.207 milliseconds (cumulative count 999966)
99.999% <= 1.303 milliseconds (cumulative count 999986)
99.999% <= 1.407 milliseconds (cumulative count 999991)
99.999% <= 1.503 milliseconds (cumulative count 999994)
100.000% <= 1.607 milliseconds (cumulative count 999999)
100.000% <= 1.703 milliseconds (cumulative count 1000000)

Summary:
  throughput summary: 87435.52 requests per second
  latency summary (msec):
          avg       min       p50       p95       p99       max
        0.046     0.008     0.047     0.079     0.111     1.623
```

## test with AOF always on

```console
root@banana:~/zys/bpftime-evaluation/redis/src# redis-benchmark -t set -n 1000000 -r 100000000 -c 5
====== SET ======                                                   
  1000000 requests completed in 79.46 seconds
  5 parallel clients
  3 bytes payload
  keep alive: 1
  host configuration "save": 3600 1 300 100 60 10000
  host configuration "appendonly": yes
  multi-thread: no

Latency by percentile distribution:
0.000% <= 0.079 milliseconds (cumulative count 1)
50.000% <= 0.263 milliseconds (cumulative count 509905)
75.000% <= 0.479 milliseconds (cumulative count 754336)
87.500% <= 0.679 milliseconds (cumulative count 876501)
93.750% <= 0.895 milliseconds (cumulative count 939148)
96.875% <= 1.023 milliseconds (cumulative count 968773)
98.438% <= 1.111 milliseconds (cumulative count 984447)
99.219% <= 1.223 milliseconds (cumulative count 992357)
99.609% <= 1.319 milliseconds (cumulative count 996306)
99.805% <= 1.407 milliseconds (cumulative count 998069)
99.902% <= 1.567 milliseconds (cumulative count 999031)
99.951% <= 1.847 milliseconds (cumulative count 999515)
99.976% <= 2.127 milliseconds (cumulative count 999758)
99.988% <= 2.311 milliseconds (cumulative count 999880)
99.994% <= 2.599 milliseconds (cumulative count 999939)
99.997% <= 3.663 milliseconds (cumulative count 999970)
99.998% <= 4.863 milliseconds (cumulative count 999985)
99.999% <= 6.647 milliseconds (cumulative count 999993)
100.000% <= 8.287 milliseconds (cumulative count 999997)
100.000% <= 8.335 milliseconds (cumulative count 999999)
100.000% <= 8.375 milliseconds (cumulative count 1000000)
100.000% <= 8.375 milliseconds (cumulative count 1000000)

Cumulative distribution of latencies:
0.005% <= 0.103 milliseconds (cumulative count 53)
15.417% <= 0.207 milliseconds (cumulative count 154168)
61.817% <= 0.303 milliseconds (cumulative count 618172)
70.665% <= 0.407 milliseconds (cumulative count 706653)
76.838% <= 0.503 milliseconds (cumulative count 768378)
83.953% <= 0.607 milliseconds (cumulative count 839525)
88.712% <= 0.703 milliseconds (cumulative count 887117)
92.012% <= 0.807 milliseconds (cumulative count 920116)
94.084% <= 0.903 milliseconds (cumulative count 940837)
96.454% <= 1.007 milliseconds (cumulative count 964538)
98.360% <= 1.103 milliseconds (cumulative count 983598)
99.153% <= 1.207 milliseconds (cumulative count 991525)
99.585% <= 1.303 milliseconds (cumulative count 995848)
99.807% <= 1.407 milliseconds (cumulative count 998069)
99.878% <= 1.503 milliseconds (cumulative count 998776)
99.914% <= 1.607 milliseconds (cumulative count 999143)
99.931% <= 1.703 milliseconds (cumulative count 999306)
99.947% <= 1.807 milliseconds (cumulative count 999468)
99.956% <= 1.903 milliseconds (cumulative count 999564)
99.966% <= 2.007 milliseconds (cumulative count 999663)
99.974% <= 2.103 milliseconds (cumulative count 999740)
99.995% <= 3.103 milliseconds (cumulative count 999953)
99.997% <= 4.103 milliseconds (cumulative count 999973)
99.999% <= 5.103 milliseconds (cumulative count 999985)
99.999% <= 6.103 milliseconds (cumulative count 999986)
99.999% <= 7.103 milliseconds (cumulative count 999993)
99.999% <= 8.103 milliseconds (cumulative count 999995)
100.000% <= 9.103 milliseconds (cumulative count 1000000)

Summary:
  throughput summary: 12584.47 requests per second
  latency summary (msec):
          avg       min       p50       p95       p99       max
        0.379     0.072     0.263     0.951     1.183     8.375
```

## summary

The additional tests you conducted compare Redis performance with and without AOF (Append Only File) enabled. Here's a comprehensive summary of all the results, including these new tests:

### Tests with Various Configurations of IO_uring Hotpatching

1. **Batch Size = 48**:
   - Throughput: 52,562.42 requests/sec
   - Average Latency: 0.083 ms
   - Max Latency: 2.143 ms

2. **Batch Size = 24**:
   - Throughput: 50,309.40 requests/sec
   - Average Latency: 0.087 ms
   - Max Latency: 5.495 ms

3. **Batch Size = 12**:
   - Throughput: 48,081.55 requests/sec
   - Average Latency: 0.090 ms
   - Max Latency: 6.575 ms

4. **Batch Size = 3**:
   - Throughput: 43,200.27 requests/sec
   - Average Latency: 0.104 ms
   - Max Latency: 3.159 ms

5. **Batch Size = 1 (Sync)**:
   - Throughput: 22,324.42 requests/sec
   - Average Latency: 0.212 ms
   - Max Latency: 3.119 ms

### Tests Without IO_uring Hotpatching

1. **Without Hotpatch, No AOF**:
   - Throughput: 87,435.52 requests/sec
   - Average Latency: 0.046 ms
   - Max Latency: 1.623 ms

2. **Without Hotpatch, AOF Always On**:
   - Throughput: 12,584.47 requests/sec
   - Average Latency: 0.379 ms
   - Max Latency: 8.375 ms

### Comparative Analysis

- **IO_uring Hotpatching**: Implementing IO_uring hotpatching substantially improves throughput and reduces latency across various batch sizes. The best performance is observed with a batch size of 48.
- **AOF Impact**: The presence of AOF (Append Only File), a persistence mode in Redis, significantly impacts performance. When AOF is turned off, the throughput nearly quadruples, and average latency decreases significantly compared to when AOF is always on.
- **Optimal Settings**: The optimal configuration for maximum throughput and minimal latency appears to be using IO_uring hotpatching with a larger batch size, and disabling AOF yields even better performance.
- **Trade-offs**: There are trade-offs to consider between data durability (offered by AOF) and performance. Disabling AOF improves performance but at the cost of potentially losing recent data in case of a crash.

### Conclusion

Your testing reveals that both the implementation of IO_uring hotpatching and the configuration of AOF in Redis significantly influence its performance. While hotpatching and larger batch sizes enhance throughput and reduce latency, disabling AOF leads to the highest performance gains, demonstrating the impact of persistence mechanisms on Redis's performance. These insights are valuable for optimizing Redis configurations based on specific requirements for data persistence and system responsiveness.