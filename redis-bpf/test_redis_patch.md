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
root@banana:~/zys/bpftime-evaluation/redis/src# ./redis-benchmark -t set -n 1000000 -r 100000000 -c 5

                      ====== SET ======
  1000000 requests completed in 41.12 seconds
  5 parallel clients
  3 bytes payload
  keep alive: 1
  host configuration "save": 3600 1 300 100 60 10000
  host configuration "appendonly": yes
  multi-thread: no

Latency by percentile distribution:
0.000% <= 0.079 milliseconds (cumulative count 7)
50.000% <= 0.175 milliseconds (cumulative count 550934)
75.000% <= 0.199 milliseconds (cumulative count 774932)
87.500% <= 0.231 milliseconds (cumulative count 884552)
93.750% <= 0.279 milliseconds (cumulative count 942253)
96.875% <= 0.359 milliseconds (cumulative count 969277)
98.438% <= 0.535 milliseconds (cumulative count 984884)
99.219% <= 0.687 milliseconds (cumulative count 992300)
99.609% <= 0.895 milliseconds (cumulative count 996198)
99.805% <= 1.063 milliseconds (cumulative count 998114)
99.902% <= 1.255 milliseconds (cumulative count 999049)
99.951% <= 1.335 milliseconds (cumulative count 999517)
99.976% <= 1.471 milliseconds (cumulative count 999757)
99.988% <= 1.735 milliseconds (cumulative count 999878)
99.994% <= 2.079 milliseconds (cumulative count 999939)
99.997% <= 2.999 milliseconds (cumulative count 999970)
99.998% <= 3.655 milliseconds (cumulative count 999985)
99.999% <= 3.959 milliseconds (cumulative count 999993)
100.000% <= 6.743 milliseconds (cumulative count 999997)
100.000% <= 6.959 milliseconds (cumulative count 999999)
100.000% <= 7.015 milliseconds (cumulative count 1000000)
100.000% <= 7.015 milliseconds (cumulative count 1000000)

Cumulative distribution of latencies:
0.167% <= 0.103 milliseconds (cumulative count 1667)
81.380% <= 0.207 milliseconds (cumulative count 813796)
95.465% <= 0.303 milliseconds (cumulative count 954645)
97.504% <= 0.407 milliseconds (cumulative count 975037)
98.257% <= 0.503 milliseconds (cumulative count 982573)
98.939% <= 0.607 milliseconds (cumulative count 989389)
99.272% <= 0.703 milliseconds (cumulative count 992722)
99.488% <= 0.807 milliseconds (cumulative count 994882)
99.629% <= 0.903 milliseconds (cumulative count 996288)
99.747% <= 1.007 milliseconds (cumulative count 997471)
99.843% <= 1.103 milliseconds (cumulative count 998432)
99.883% <= 1.207 milliseconds (cumulative count 998835)
99.938% <= 1.303 milliseconds (cumulative count 999383)
99.966% <= 1.407 milliseconds (cumulative count 999665)
99.978% <= 1.503 milliseconds (cumulative count 999779)
99.983% <= 1.607 milliseconds (cumulative count 999829)
99.987% <= 1.703 milliseconds (cumulative count 999868)
99.989% <= 1.807 milliseconds (cumulative count 999893)
99.991% <= 1.903 milliseconds (cumulative count 999907)
99.993% <= 2.007 milliseconds (cumulative count 999933)
99.994% <= 2.103 milliseconds (cumulative count 999942)
99.997% <= 3.103 milliseconds (cumulative count 999970)
99.999% <= 4.103 milliseconds (cumulative count 999993)
99.999% <= 5.103 milliseconds (cumulative count 999995)
100.000% <= 7.103 milliseconds (cumulative count 1000000)

Summary:
  throughput summary: 24321.43 requests per second
  latency summary (msec):
          avg       min       p50       p95       p99       max
        0.192     0.072     0.175     0.295     0.623     7.015
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