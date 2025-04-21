# Benchmark on bpftime-nginx

```
wrk -t 20 -c 250 http://127.0.0.1:9023
```

With new bpf program

- Worker process count 10
## Without bpftime module

```console
wrk -t 20 -c 250 http://127.0.0.1:9023/mikunot/foundexception
Running 10s test @ http://127.0.0.1:9023/mikunot/foundexception
  20 threads and 250 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     1.62ms  709.09us  17.55ms   71.76%
    Req/Sec     7.43k     2.55k   14.68k    43.50%
  1489642 requests in 10.10s, 441.81MB read
Requests/sec: 147498.81
Transfer/sec:     43.75MB
```

## With bpftime module, enabled, url that could pass check, with metrics
```console
wrk -c 250 -t 20  http:
//127.0.0.1:9023/mikunot/foundexception
Running 10s test @ http://127.0.0.1:9023/mikunot/foundexception
  20 threads and 250 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     2.71ms  643.50us  19.12ms   79.74%
    Req/Sec     4.44k   322.08     5.94k    75.11%
  891195 requests in 10.10s, 264.32MB read
Requests/sec:  88239.50
Transfer/sec:     26.17MB
```

## With bpftime module, enabled, url that could pass check, without metrics
```console
wrk -t 20 -c 250 http://127.0.0.1:9023/mikunot/foundexception
Running 10s test @ http://127.0.0.1:9023/mikunot/foundexception
  20 threads and 250 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     1.78ms  764.06us  18.62ms   79.61%
    Req/Sec     6.81k     1.96k   11.05k    53.54%
  1367036 requests in 10.10s, 405.45MB read
Requests/sec: 135351.92
Transfer/sec:     40.14MB
```

## With lua module, url that passes the check
```console
wrk -t 20 -c 250 http://127.0.0.1:9023/mikunot/foundexception/
Running 10s test @ http://127.0.0.1:9023/mikunot/foundexception/
  20 threads and 250 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     2.09ms    0.89ms  17.19ms   76.06%
    Req/Sec     5.77k     1.70k    9.74k    54.32%
  1157420 requests in 10.10s, 343.28MB read
Requests/sec: 114598.83
Transfer/sec:     33.99MB
```

## With wasm module, enabled, url that passes the check (without optimization)
```console
wrk -t 20 -c 250 http://127.0.0.1:9023/mikunot/foundexception/
Running 10s test @ http://127.0.0.1:9023/mikunot/foundexception/
  20 threads and 250 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     9.37ms   43.23ms 401.74ms   96.86%
    Req/Sec     5.51k     1.74k    9.07k    71.10%
  1073360 requests in 10.10s, 318.35MB read
Requests/sec: 106281.29
Transfer/sec:     31.52MB
```

## With wasm module, enabled, url that passes the check (with optimization)
```console
wrk -t 20 -c 250 http://127.0.0.1:9023/mikunot/foundexception/
Running 10s test @ http://127.0.0.1:9023/mikunot/foundexception/
  20 threads and 250 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     1.87ms    0.91ms  20.83ms   78.61%
    Req/Sec     6.52k     1.98k   12.12k    57.71%
  1308897 requests in 10.10s, 388.20MB read
Requests/sec: 129598.61
Transfer/sec:     38.44MB
```
