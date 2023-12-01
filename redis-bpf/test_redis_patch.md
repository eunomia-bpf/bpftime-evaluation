# test redis with hotpatch iouring

## test without hotpatch iouring

start redis

```console
root@banana:~/zys/bpftime-evaluation/redis/s
rc# ./redis-server ../redis.conf 
541115:C 01 Dec 2023 06:28:00.848 * oO0OoO0OoO0Oo Redis is starting oO0OoO0OoO0Oo
541115:C 01 Dec 2023 06:28:00.848 * Redis version=255.255.255, bits=64, commit=ba3237f0, modified=1, pid=541115, just started
541115:C 01 Dec 2023 06:28:00.848 * Configuration loaded
541115:M 01 Dec 2023 06:28:00.849 * monotonic clock: POSIX clock_gettime
                _._                                                  
           _.-``__ ''-._                                             
      _.-``    `.  `_.  ''-._           Redis 255.255.255 (ba3237f0/1) 64 bit
  .-`` .-```.  ```\/    _.,_ ''-._                                  
 (    '      ,       .-`  | `,    )     Running in standalone mode
 |`-._`-...-` __...-.``-._|'` _.-'|     Port: 6379
 |    `-._   `._    /     _.-'    |     PID: 541115
  `-._    `-._  `-./  _.-'    _.-'                                   
 |`-._`-._    `-.__.-'    _.-'_.-'|                                  
 |    `-._`-._        _.-'_.-'    |           https://redis.io       
  `-._    `-._`-.__.-'_.-'    _.-'                                   
 |`-._`-._    `-.__.-'    _.-'_.-'|                                  
 |    `-._`-._        _.-'_.-'    |                                  
  `-._    `-._`-.__.-'_.-'    _.-'                                   
      `-._    `-.__.-'    _.-'                                       
          `-._        _.-'                                           
              `-.__.-'                                               

541115:M 01 Dec 2023 06:28:00.850 # Warning: Could not create server TCP listening socket ::1:6379: bind: Cannot assign requested address
541115:M 01 Dec 2023 06:28:00.852 * Server initialized
541115:M 01 Dec 2023 06:28:00.852 * Reading RDB base file on AOF loading...
541115:M 01 Dec 2023 06:28:00.852 * Loading RDB produced by version 255.255.255
541115:M 01 Dec 2023 06:28:00.852 * RDB age 19020 seconds
541115:M 01 Dec 2023 06:28:00.852 * RDB memory usage when created 0.83 Mb
541115:M 01 Dec 2023 06:28:00.852 * RDB is base AOF
541115:M 01 Dec 2023 06:28:00.852 * Done loading RDB, keys loaded: 0, keys expired: 0.
541115:M 01 Dec 2023 06:28:00.852 * DB loaded from base file appendonly.aof.1.base.rdb: 0.000 seconds
541115:M 01 Dec 2023 06:28:02.632 * DB loaded from incr file appendonly.aof.1.incr.aof: 1.780 seconds
541115:M 01 Dec 2023 06:28:02.632 * DB loaded from append only file: 1.780 seconds
541115:M 01 Dec 2023 06:28:02.632 * Opening AOF incr file appendonly.aof.1.incr.aof on server start
541115:M 01 Dec 2023 06:28:02.632 * Ready to accept connections tcp
```

start benchmark

```console
====== SET ======                                                   
  1000000 requests completed in 44.93 seconds
  5 parallel clients
  3 bytes payload
  keep alive: 1
  host configuration "save": 3600 1 300 100 60 10000
  host configuration "appendonly": yes
  multi-thread: no

Latency by percentile distribution:
0.000% <= 0.087 milliseconds (cumulative count 19)
50.000% <= 0.183 milliseconds (cumulative count 543255)
75.000% <= 0.207 milliseconds (cumulative count 766210)
87.500% <= 0.255 milliseconds (cumulative count 877891)
93.750% <= 0.359 milliseconds (cumulative count 939694)
96.875% <= 0.535 milliseconds (cumulative count 969926)
98.438% <= 0.663 milliseconds (cumulative count 984667)
99.219% <= 0.823 milliseconds (cumulative count 992395)
99.609% <= 0.935 milliseconds (cumulative count 996264)
99.805% <= 1.039 milliseconds (cumulative count 998078)
99.902% <= 1.127 milliseconds (cumulative count 999075)
99.951% <= 1.199 milliseconds (cumulative count 999531)
99.976% <= 1.271 milliseconds (cumulative count 999771)
99.988% <= 1.343 milliseconds (cumulative count 999879)
99.994% <= 1.463 milliseconds (cumulative count 999942)
99.997% <= 1.831 milliseconds (cumulative count 999970)
99.998% <= 5.255 milliseconds (cumulative count 999985)
99.999% <= 5.799 milliseconds (cumulative count 999993)
100.000% <= 6.047 milliseconds (cumulative count 999997)
100.000% <= 6.511 milliseconds (cumulative count 999999)
100.000% <= 6.535 milliseconds (cumulative count 1000000)
100.000% <= 6.535 milliseconds (cumulative count 1000000)

Cumulative distribution of latencies:
0.086% <= 0.103 milliseconds (cumulative count 865)
76.621% <= 0.207 milliseconds (cumulative count 766210)
91.056% <= 0.303 milliseconds (cumulative count 910560)
95.045% <= 0.407 milliseconds (cumulative count 950449)
96.487% <= 0.503 milliseconds (cumulative count 964869)
98.038% <= 0.607 milliseconds (cumulative count 980378)
98.708% <= 0.703 milliseconds (cumulative count 987075)
99.171% <= 0.807 milliseconds (cumulative count 991708)
99.549% <= 0.903 milliseconds (cumulative count 995486)
99.755% <= 1.007 milliseconds (cumulative count 997546)
99.886% <= 1.103 milliseconds (cumulative count 998860)
99.957% <= 1.207 milliseconds (cumulative count 999567)
99.983% <= 1.303 milliseconds (cumulative count 999827)
99.992% <= 1.407 milliseconds (cumulative count 999920)
99.996% <= 1.503 milliseconds (cumulative count 999955)
99.997% <= 1.607 milliseconds (cumulative count 999966)
99.997% <= 1.703 milliseconds (cumulative count 999969)
99.997% <= 1.903 milliseconds (cumulative count 999974)
99.997% <= 3.103 milliseconds (cumulative count 999975)
99.998% <= 5.103 milliseconds (cumulative count 999980)
100.000% <= 6.103 milliseconds (cumulative count 999997)
100.000% <= 7.103 milliseconds (cumulative count 1000000)

Summary:
  throughput summary: 22258.33 requests per second
  latency summary (msec):
          avg       min       p50       p95       p99       max
        0.212     0.080     0.183     0.407     0.767     6.535

root@banana:~/zys/bpftime-evaluation/redis/src# ./redis-benchmark -t set -n 1000000 -r 100000000 -c 5
====== SET ======                                                   
  1000000 requests completed in 48.06 seconds
  5 parallel clients
  3 bytes payload
  keep alive: 1
  host configuration "save": 3600 1 300 100 60 10000
  host configuration "appendonly": yes
  multi-thread: no

Latency by percentile distribution:
0.000% <= 0.087 milliseconds (cumulative count 20)
50.000% <= 0.183 milliseconds (cumulative count 550214)
75.000% <= 0.215 milliseconds (cumulative count 757778)
87.500% <= 0.303 milliseconds (cumulative count 877976)
93.750% <= 0.479 milliseconds (cumulative count 939011)
96.875% <= 0.631 milliseconds (cumulative count 968956)
98.438% <= 0.823 milliseconds (cumulative count 984840)
99.219% <= 0.975 milliseconds (cumulative count 992242)
99.609% <= 1.119 milliseconds (cumulative count 996155)
99.805% <= 1.279 milliseconds (cumulative count 998094)
99.902% <= 1.399 milliseconds (cumulative count 999042)
99.951% <= 1.671 milliseconds (cumulative count 999512)
99.976% <= 1.975 milliseconds (cumulative count 999759)
99.988% <= 2.311 milliseconds (cumulative count 999878)
99.994% <= 3.327 milliseconds (cumulative count 999939)
99.997% <= 4.583 milliseconds (cumulative count 999970)
99.998% <= 5.903 milliseconds (cumulative count 999985)
99.999% <= 8.623 milliseconds (cumulative count 999993)
100.000% <= 9.751 milliseconds (cumulative count 999997)
100.000% <= 9.879 milliseconds (cumulative count 999999)
100.000% <= 9.895 milliseconds (cumulative count 1000000)
100.000% <= 9.895 milliseconds (cumulative count 1000000)

Cumulative distribution of latencies:
0.076% <= 0.103 milliseconds (cumulative count 763)
72.355% <= 0.207 milliseconds (cumulative count 723550)
87.798% <= 0.303 milliseconds (cumulative count 877976)
92.388% <= 0.407 milliseconds (cumulative count 923879)
94.404% <= 0.503 milliseconds (cumulative count 944038)
96.569% <= 0.607 milliseconds (cumulative count 965688)
97.627% <= 0.703 milliseconds (cumulative count 976269)
98.382% <= 0.807 milliseconds (cumulative count 983817)
98.949% <= 0.903 milliseconds (cumulative count 989486)
99.337% <= 1.007 milliseconds (cumulative count 993371)
99.588% <= 1.103 milliseconds (cumulative count 995879)
99.728% <= 1.207 milliseconds (cumulative count 997282)
99.844% <= 1.303 milliseconds (cumulative count 998440)
99.908% <= 1.407 milliseconds (cumulative count 999081)
99.933% <= 1.503 milliseconds (cumulative count 999325)
99.944% <= 1.607 milliseconds (cumulative count 999444)
99.955% <= 1.703 milliseconds (cumulative count 999550)
99.964% <= 1.807 milliseconds (cumulative count 999635)
99.971% <= 1.903 milliseconds (cumulative count 999713)
99.978% <= 2.007 milliseconds (cumulative count 999781)
99.982% <= 2.103 milliseconds (cumulative count 999821)
99.993% <= 3.103 milliseconds (cumulative count 999935)
99.996% <= 4.103 milliseconds (cumulative count 999958)
99.997% <= 5.103 milliseconds (cumulative count 999975)
99.999% <= 6.103 milliseconds (cumulative count 999985)
99.999% <= 8.103 milliseconds (cumulative count 999990)
99.999% <= 9.103 milliseconds (cumulative count 999995)
100.000% <= 10.103 milliseconds (cumulative count 1000000)

Summary:
  throughput summary: 20806.03 requests per second
  latency summary (msec):
          avg       min       p50       p95       p99       max
        0.227     0.080     0.183     0.535     0.919     9.895

root@banana:~/zys/bpftime-evaluation/redis/src# 
```

## test with hotpatch iouring

start syscal server

```sh
root@banana:~/zys/bpftime-evaluation# LD_PRELOAD=/root/zys/bpftime/build/runtime/syscall-server/libbpftime-syscall-server.so redis-bpf/bpf/rocksdb
```

batch size = 48

start redis

```console
LD_PRELOAD=/root/zys/bpftime/build/runtime/agent/libbpftime-agent.so ./redis-server ../redis.conf 
[2023-12-01 06:29:52.723] [info] [agent.cpp:66] Entering bpftime agent
[2023-12-01 06:29:52.724] [info] [bpftime_shm_internal.cpp:597] Global shm constructed. shm_open_type 1 for bpftime_maps_shm
[2023-12-01 06:29:52.724] [info] [agent.cpp:81] Initializing agent..
[2023-12-01 06:29:52][info][542389] Executable path: /root/zys/bpftime-evaluation/redis/src/redis-server
[2023-12-01 06:29:52][info][542389] Executable path: /root/zys/bpftime-evaluation/redis/src/redis-server
[2023-12-01 06:29:52][info][542389] Executable path: /root/zys/bpftime-evaluation/redis/src/redis-server
[2023-12-01 06:29:52][info][542389] Attach successfully
start called and init io_uring
542389:C 01 Dec 2023 06:29:52.731 * oO0OoO0OoO0Oo Redis is starting oO0OoO0OoO0Oo
542389:C 01 Dec 2023 06:29:52.731 * Redis version=255.255.255, bits=64, commit=ba3237f0, modified=1, pid=542389, just started
542389:C 01 Dec 2023 06:29:52.731 * Configuration loaded
542389:M 01 Dec 2023 06:29:52.732 * monotonic clock: POSIX clock_gettime
                _._                                                  
           _.-``__ ''-._                                             
      _.-``    `.  `_.  ''-._           Redis 255.255.255 (ba3237f0/1) 64 bit
  .-`` .-```.  ```\/    _.,_ ''-._                                  
 (    '      ,       .-`  | `,    )     Running in standalone mode
 |`-._`-...-` __...-.``-._|'` _.-'|     Port: 6379
 |    `-._   `._    /     _.-'    |     PID: 542389
  `-._    `-._  `-./  _.-'    _.-'                                   
 |`-._`-._    `-.__.-'    _.-'_.-'|                                  
 |    `-._`-._        _.-'_.-'    |           https://redis.io       
  `-._    `-._`-.__.-'_.-'    _.-'                                   
 |`-._`-._    `-.__.-'    _.-'_.-'|                                  
 |    `-._`-._        _.-'_.-'    |                                  
  `-._    `-._`-.__.-'_.-'    _.-'                                   
      `-._    `-.__.-'    _.-'                                       
          `-._        _.-'                                           
              `-.__.-'                                               

542389:M 01 Dec 2023 06:29:52.732 # Warning: Could not create server TCP listening socket ::1:6379: bind: Cannot assign requested address
542389:M 01 Dec 2023 06:29:52.734 * Server initialized
542389:M 01 Dec 2023 06:29:52.735 * Creating AOF base file appendonly.aof.1.base.rdb on server start
542389:M 01 Dec 2023 06:29:52.735 * Creating AOF incr file appendonly.aof.1.incr.aof on server start
542389:M 01 Dec 2023 06:29:52.735 * Ready to accept connections tcp
```

start benchmark

```console
# ./redis-benchmark -t set -n 1000000 -r 
100000000 -c 5
====== SET ======                                                   
  1000000 requests completed in 19.02 seconds
  5 parallel clients
  3 bytes payload
  keep alive: 1
  host configuration "save": 3600 1 300 100 60 10000
  host configuration "appendonly": yes
  multi-thread: no

Latency by percentile distribution:
0.000% <= 0.023 milliseconds (cumulative count 2205)
50.000% <= 0.063 milliseconds (cumulative count 588356)
75.000% <= 0.079 milliseconds (cumulative count 755221)
87.500% <= 0.103 milliseconds (cumulative count 879233)
93.750% <= 0.151 milliseconds (cumulative count 940182)
96.875% <= 0.343 milliseconds (cumulative count 969038)
98.438% <= 0.543 milliseconds (cumulative count 984948)
99.219% <= 0.647 milliseconds (cumulative count 992652)
99.609% <= 0.727 milliseconds (cumulative count 996324)
99.805% <= 0.799 milliseconds (cumulative count 998074)
99.902% <= 0.879 milliseconds (cumulative count 999074)
99.951% <= 0.983 milliseconds (cumulative count 999515)
99.976% <= 1.167 milliseconds (cumulative count 999757)
99.988% <= 1.223 milliseconds (cumulative count 999878)
99.994% <= 1.319 milliseconds (cumulative count 999941)
99.997% <= 1.447 milliseconds (cumulative count 999970)
99.998% <= 1.767 milliseconds (cumulative count 999986)
99.999% <= 1.831 milliseconds (cumulative count 999994)
100.000% <= 2.119 milliseconds (cumulative count 999999)
100.000% <= 2.143 milliseconds (cumulative count 1000000)
100.000% <= 2.143 milliseconds (cumulative count 1000000)

Cumulative distribution of latencies:
87.923% <= 0.103 milliseconds (cumulative count 879233)
95.038% <= 0.207 milliseconds (cumulative count 950381)
96.459% <= 0.303 milliseconds (cumulative count 964591)
97.488% <= 0.407 milliseconds (cumulative count 974877)
98.167% <= 0.503 milliseconds (cumulative count 981672)
99.005% <= 0.607 milliseconds (cumulative count 990047)
99.542% <= 0.703 milliseconds (cumulative count 995417)
99.821% <= 0.807 milliseconds (cumulative count 998207)
99.925% <= 0.903 milliseconds (cumulative count 999253)
99.957% <= 1.007 milliseconds (cumulative count 999570)
99.971% <= 1.103 milliseconds (cumulative count 999708)
99.986% <= 1.207 milliseconds (cumulative count 999857)
99.993% <= 1.303 milliseconds (cumulative count 999933)
99.997% <= 1.407 milliseconds (cumulative count 999966)
99.997% <= 1.503 milliseconds (cumulative count 999972)
99.997% <= 1.607 milliseconds (cumulative count 999975)
99.998% <= 1.703 milliseconds (cumulative count 999981)
99.999% <= 1.807 milliseconds (cumulative count 999992)
99.999% <= 1.903 milliseconds (cumulative count 999995)
100.000% <= 3.103 milliseconds (cumulative count 1000000)

Summary:
  throughput summary: 52562.42 requests per second
  latency summary (msec):
          avg       min       p50       p95       p99       max
        0.083     0.016     0.063     0.207     0.607     2.143

root@banana:~/zys/bpftime-evaluation/redis/src# 
```

batch size = 24

```console
root@banana:~/zys/bpftime-evaluation/redis/src# ./redis-benchmark -t set -n 1000000 -r 100000000 -c 5
====== SET ======                                                   
  1000000 requests completed in 19.88 seconds
  5 parallel clients
  3 bytes payload
  keep alive: 1
  host configuration "save": 3600 1 300 100 60 10000
  host configuration "appendonly": yes
  multi-thread: no

Latency by percentile distribution:
0.000% <= 0.023 milliseconds (cumulative count 6303)
50.000% <= 0.063 milliseconds (cumulative count 617290)
75.000% <= 0.079 milliseconds (cumulative count 763635)
87.500% <= 0.143 milliseconds (cumulative count 882580)
93.750% <= 0.223 milliseconds (cumulative count 941508)
96.875% <= 0.319 milliseconds (cumulative count 968786)
98.438% <= 0.535 milliseconds (cumulative count 984793)
99.219% <= 0.647 milliseconds (cumulative count 992567)
99.609% <= 0.735 milliseconds (cumulative count 996357)
99.805% <= 0.807 milliseconds (cumulative count 998051)
99.902% <= 0.887 milliseconds (cumulative count 999039)
99.951% <= 0.983 milliseconds (cumulative count 999535)
99.976% <= 1.175 milliseconds (cumulative count 999774)
99.988% <= 1.335 milliseconds (cumulative count 999881)
99.994% <= 1.823 milliseconds (cumulative count 999939)
99.997% <= 2.799 milliseconds (cumulative count 999970)
99.998% <= 4.343 milliseconds (cumulative count 999985)
99.999% <= 5.135 milliseconds (cumulative count 999993)
100.000% <= 5.439 milliseconds (cumulative count 999997)
100.000% <= 5.479 milliseconds (cumulative count 999999)
100.000% <= 5.495 milliseconds (cumulative count 1000000)
100.000% <= 5.495 milliseconds (cumulative count 1000000)

Cumulative distribution of latencies:
84.445% <= 0.103 milliseconds (cumulative count 844450)
93.216% <= 0.207 milliseconds (cumulative count 932159)
96.692% <= 0.303 milliseconds (cumulative count 966923)
97.695% <= 0.407 milliseconds (cumulative count 976949)
98.270% <= 0.503 milliseconds (cumulative count 982701)
99.016% <= 0.607 milliseconds (cumulative count 990165)
99.511% <= 0.703 milliseconds (cumulative count 995110)
99.805% <= 0.807 milliseconds (cumulative count 998051)
99.915% <= 0.903 milliseconds (cumulative count 999149)
99.959% <= 1.007 milliseconds (cumulative count 999594)
99.969% <= 1.103 milliseconds (cumulative count 999691)
99.983% <= 1.207 milliseconds (cumulative count 999830)
99.987% <= 1.303 milliseconds (cumulative count 999870)
99.990% <= 1.407 milliseconds (cumulative count 999897)
99.991% <= 1.503 milliseconds (cumulative count 999906)
99.991% <= 1.607 milliseconds (cumulative count 999911)
99.992% <= 1.703 milliseconds (cumulative count 999924)
99.994% <= 1.807 milliseconds (cumulative count 999936)
99.995% <= 1.903 milliseconds (cumulative count 999952)
99.995% <= 2.007 milliseconds (cumulative count 999953)
99.996% <= 2.103 milliseconds (cumulative count 999958)
99.997% <= 3.103 milliseconds (cumulative count 999971)
99.998% <= 4.103 milliseconds (cumulative count 999977)
99.999% <= 5.103 milliseconds (cumulative count 999992)
100.000% <= 6.103 milliseconds (cumulative count 1000000)

Summary:
  throughput summary: 50309.40 requests per second
  latency summary (msec):
          avg       min       p50       p95       p99       max
        0.087     0.016     0.063     0.247     0.607     5.495

root@banana:~/zys/bpftime-evaluation/redis/src# 
```

batch size = 12

```console
                                                   ====== SET ======
  1000000 requests completed in 20.80 seconds
  5 parallel clients
  3 bytes payload
  keep alive: 1
  host configuration "save": 3600 1 300 100 60 10000
  host configuration "appendonly": yes
  multi-thread: no

Latency by percentile distribution:
0.000% <= 0.023 milliseconds (cumulative count 996)
50.000% <= 0.063 milliseconds (cumulative count 504806)
75.000% <= 0.095 milliseconds (cumulative count 763181)
87.500% <= 0.167 milliseconds (cumulative count 887286)
93.750% <= 0.207 milliseconds (cumulative count 945021)
96.875% <= 0.255 milliseconds (cumulative count 969393)
98.438% <= 0.335 milliseconds (cumulative count 985287)
99.219% <= 0.407 milliseconds (cumulative count 992378)
99.609% <= 0.519 milliseconds (cumulative count 996237)
99.805% <= 0.591 milliseconds (cumulative count 998162)
99.902% <= 0.687 milliseconds (cumulative count 999061)
99.951% <= 1.119 milliseconds (cumulative count 999523)
99.976% <= 1.199 milliseconds (cumulative count 999768)
99.988% <= 1.295 milliseconds (cumulative count 999880)
99.994% <= 1.391 milliseconds (cumulative count 999944)
99.997% <= 1.567 milliseconds (cumulative count 999970)
99.998% <= 3.527 milliseconds (cumulative count 999985)
99.999% <= 4.271 milliseconds (cumulative count 999993)
100.000% <= 6.503 milliseconds (cumulative count 999997)
100.000% <= 6.559 milliseconds (cumulative count 999999)
100.000% <= 6.575 milliseconds (cumulative count 1000000)
100.000% <= 6.575 milliseconds (cumulative count 1000000)

Cumulative distribution of latencies:
77.400% <= 0.103 milliseconds (cumulative count 774004)
94.502% <= 0.207 milliseconds (cumulative count 945021)
97.989% <= 0.303 milliseconds (cumulative count 979890)
99.238% <= 0.407 milliseconds (cumulative count 992378)
99.571% <= 0.503 milliseconds (cumulative count 995709)
99.839% <= 0.607 milliseconds (cumulative count 998389)
99.914% <= 0.703 milliseconds (cumulative count 999140)
99.939% <= 0.807 milliseconds (cumulative count 999386)
99.946% <= 0.903 milliseconds (cumulative count 999458)
99.949% <= 1.007 milliseconds (cumulative count 999489)
99.951% <= 1.103 milliseconds (cumulative count 999506)
99.979% <= 1.207 milliseconds (cumulative count 999786)
99.989% <= 1.303 milliseconds (cumulative count 999887)
99.995% <= 1.407 milliseconds (cumulative count 999950)
99.996% <= 1.503 milliseconds (cumulative count 999964)
99.997% <= 1.607 milliseconds (cumulative count 999973)
99.997% <= 1.703 milliseconds (cumulative count 999975)
99.998% <= 1.903 milliseconds (cumulative count 999980)
99.999% <= 4.103 milliseconds (cumulative count 999991)
99.999% <= 5.103 milliseconds (cumulative count 999995)
100.000% <= 7.103 milliseconds (cumulative count 1000000)

Summary:
  throughput summary: 48081.55 requests per second
  latency summary (msec):
          avg       min       p50       p95       p99       max
        0.090     0.016     0.063     0.215     0.375     6.575
```

batch size = 3

```console
====== SET ======
  1000000 requests completed in 23.15 seconds
  5 parallel clients
  3 bytes payload
  keep alive: 1
  host configuration "save": 3600 1 300 100 60 10000
  host configuration "appendonly": yes
  multi-thread: no

Latency by percentile distribution:
0.000% <= 0.023 milliseconds (cumulative count 625)
50.000% <= 0.111 milliseconds (cumulative count 536165)
75.000% <= 0.143 milliseconds (cumulative count 804367)
87.500% <= 0.159 milliseconds (cumulative count 887410)
93.750% <= 0.183 milliseconds (cumulative count 951086)
96.875% <= 0.199 milliseconds (cumulative count 970850)
98.438% <= 0.223 milliseconds (cumulative count 984595)
99.219% <= 0.255 milliseconds (cumulative count 992488)
99.609% <= 0.295 milliseconds (cumulative count 996099)
99.805% <= 0.359 milliseconds (cumulative count 998174)
99.902% <= 0.455 milliseconds (cumulative count 999035)
99.951% <= 0.623 milliseconds (cumulative count 999516)
99.976% <= 1.295 milliseconds (cumulative count 999768)
99.988% <= 1.383 milliseconds (cumulative count 999882)
99.994% <= 1.463 milliseconds (cumulative count 999939)
99.997% <= 1.671 milliseconds (cumulative count 999971)
99.998% <= 1.879 milliseconds (cumulative count 999985)
99.999% <= 2.231 milliseconds (cumulative count 999993)
100.000% <= 3.095 milliseconds (cumulative count 999997)
100.000% <= 3.143 milliseconds (cumulative count 999999)
100.000% <= 3.159 milliseconds (cumulative count 1000000)
100.000% <= 3.159 milliseconds (cumulative count 1000000)

Cumulative distribution of latencies:
48.441% <= 0.103 milliseconds (cumulative count 484407)
97.675% <= 0.207 milliseconds (cumulative count 976755)
99.652% <= 0.303 milliseconds (cumulative count 996519)
99.872% <= 0.407 milliseconds (cumulative count 998722)
99.920% <= 0.503 milliseconds (cumulative count 999199)
99.949% <= 0.607 milliseconds (cumulative count 999489)
99.960% <= 0.703 milliseconds (cumulative count 999600)
99.965% <= 0.807 milliseconds (cumulative count 999652)
99.967% <= 0.903 milliseconds (cumulative count 999673)
99.969% <= 1.007 milliseconds (cumulative count 999695)
99.971% <= 1.103 milliseconds (cumulative count 999706)
99.971% <= 1.207 milliseconds (cumulative count 999712)
99.978% <= 1.303 milliseconds (cumulative count 999779)
99.991% <= 1.407 milliseconds (cumulative count 999908)
99.995% <= 1.503 milliseconds (cumulative count 999950)
99.996% <= 1.607 milliseconds (cumulative count 999962)
99.997% <= 1.703 milliseconds (cumulative count 999972)
99.998% <= 1.807 milliseconds (cumulative count 999979)
99.999% <= 1.903 milliseconds (cumulative count 999985)
99.999% <= 2.103 milliseconds (cumulative count 999990)
100.000% <= 3.103 milliseconds (cumulative count 999998)
100.000% <= 4.103 milliseconds (cumulative count 1000000)

Summary:
  throughput summary: 43200.27 requests per second
  latency summary (msec):
          avg       min       p50       p95       p99       max
        0.104     0.016     0.111     0.183     0.247     3.159
```

batch size = 1 (sync)

```console
                           ====== SET ======
  1000000 requests completed in 44.79 seconds
  5 parallel clients
  3 bytes payload
  keep alive: 1
  host configuration "save": 3600 1 300 100 60 10000
  host configuration "appendonly": yes
  multi-thread: no

Latency by percentile distribution:
0.000% <= 0.087 milliseconds (cumulative count 1)
50.000% <= 0.207 milliseconds (cumulative count 515793)
75.000% <= 0.231 milliseconds (cumulative count 813101)
87.500% <= 0.247 milliseconds (cumulative count 902586)
93.750% <= 0.263 milliseconds (cumulative count 946513)
96.875% <= 0.279 milliseconds (cumulative count 968886)
98.438% <= 0.303 milliseconds (cumulative count 984458)
99.219% <= 0.335 milliseconds (cumulative count 992362)
99.609% <= 0.383 milliseconds (cumulative count 996319)
99.805% <= 0.463 milliseconds (cumulative count 998146)
99.902% <= 0.623 milliseconds (cumulative count 999061)
99.951% <= 0.839 milliseconds (cumulative count 999513)
99.976% <= 1.359 milliseconds (cumulative count 999768)
99.988% <= 1.391 milliseconds (cumulative count 999889)
99.994% <= 1.431 milliseconds (cumulative count 999946)
99.997% <= 1.727 milliseconds (cumulative count 999971)
99.998% <= 2.255 milliseconds (cumulative count 999985)
99.999% <= 2.335 milliseconds (cumulative count 999993)
100.000% <= 2.391 milliseconds (cumulative count 999997)
100.000% <= 3.103 milliseconds (cumulative count 999999)
100.000% <= 3.119 milliseconds (cumulative count 1000000)
100.000% <= 3.119 milliseconds (cumulative count 1000000)

Cumulative distribution of latencies:
0.011% <= 0.103 milliseconds (cumulative count 108)
51.579% <= 0.207 milliseconds (cumulative count 515793)
98.446% <= 0.303 milliseconds (cumulative count 984458)
99.712% <= 0.407 milliseconds (cumulative count 997122)
99.855% <= 0.503 milliseconds (cumulative count 998545)
99.899% <= 0.607 milliseconds (cumulative count 998988)
99.932% <= 0.703 milliseconds (cumulative count 999323)
99.949% <= 0.807 milliseconds (cumulative count 999493)
99.955% <= 0.903 milliseconds (cumulative count 999548)
99.957% <= 1.007 milliseconds (cumulative count 999569)
99.958% <= 1.103 milliseconds (cumulative count 999576)
99.958% <= 1.207 milliseconds (cumulative count 999584)
99.960% <= 1.303 milliseconds (cumulative count 999595)
99.991% <= 1.407 milliseconds (cumulative count 999913)
99.996% <= 1.503 milliseconds (cumulative count 999965)
99.997% <= 1.607 milliseconds (cumulative count 999968)
99.997% <= 1.703 milliseconds (cumulative count 999969)
99.998% <= 1.807 milliseconds (cumulative count 999978)
99.998% <= 1.903 milliseconds (cumulative count 999982)
100.000% <= 3.103 milliseconds (cumulative count 999999)
100.000% <= 4.103 milliseconds (cumulative count 1000000)

Summary:
  throughput summary: 22324.42 requests per second
  latency summary (msec):
          avg       min       p50       p95       p99       max
        0.212     0.080     0.207     0.271     0.327     3.119
```
