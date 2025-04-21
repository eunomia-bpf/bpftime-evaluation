# Benchmark on bpftime-nginx

```
wrk -t 10 -c 100 http://127.0.
0.1:9023
```

- Worker process count 10
## Without bpftime module

```console
# wrk -t 10 -c 100 http://127.0.0.1:9023
Running 10s test @ http://127.0.0.1:9023
  10 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   661.14us  232.20us   4.96ms   91.59%
    Req/Sec    15.35k     2.89k   24.81k    87.88%
  1538434 requests in 10.10s, 456.28MB read
Requests/sec: 152326.68
Transfer/sec:     45.18MB
```

## With bpftime module, but not enabled
```console
wrk -t 10 -c 100 http://127.0.0.1:9023
Running 10s test @ http://127.0.0.1:9023
  10 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   654.87us  202.93us   4.60ms   90.55%
    Req/Sec    15.39k     2.82k   21.38k    86.82%
  1545151 requests in 10.10s, 458.27MB read
Requests/sec: 152985.29
Transfer/sec:     45.37MB
```

## With bpftime module, enabled, url that could pass check
```console
wrk -t 10 -c 100 http://127.0.0.1:9023
Running 10s test @ http://127.0.0.1:9023
  10 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   635.45us  203.04us   2.87ms   90.49%
    Req/Sec    15.89k     3.08k   23.74k    86.71%
  1592880 requests in 10.10s, 472.43MB read
Requests/sec: 157708.99
Transfer/sec:     46.77MB
```

## With bpftime module, enabled, url that couldn't pass check
```console
wrk -t 10 -c 100 http://127.0.0.1:9023/X
Running 10s test @ http://127.0.0.1:9023/X
  10 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   585.51us  152.32us   2.97ms   92.74%
    Req/Sec    17.17k     2.60k   23.86k    91.27%
  1722027 requests in 10.10s, 505.81MB read
  Non-2xx or 3xx responses: 1722027
Requests/sec: 170500.27
Transfer/sec:     50.08MB
```

## With lua module, url that passes the check
```console
wrk -t 10 -c 100 http://127.0.0.1:9023
Running 10s test @ http://127.0.0.1:9023
  10 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   775.29us  280.12us   6.77ms   91.31%
    Req/Sec    13.17k     2.66k   20.04k    89.89%
  1321912 requests in 10.10s, 392.06MB read
Requests/sec: 130880.67
Transfer/sec:     38.82MB
```

## With lua module, url that couldn't pass check
```console
wrk -t 10 -c 100 http://127.0.0.1:9023/X
Running 10s test @ http://127.0.0.1:9023/X
  10 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   597.07us  190.15us   4.47ms   91.91%
    Req/Sec    16.94k     3.10k   26.58k    88.48%
  1697205 requests in 10.10s, 498.52MB read
  Non-2xx or 3xx responses: 1697205
Requests/sec: 168050.85
Transfer/sec:     49.36MB
```
