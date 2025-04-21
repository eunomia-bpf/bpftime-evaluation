# Benchmark on bpftime-nginx

```
wrk -t 10 -c 100 http://127.0.
0.1:9023
```

- Worker process count 10
## Without bpftime module

```console
 wrk -c 100 -t 10 http://127.0.0.1:9023
Running 10s test @ http://127.0.0.1:9023
  10 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   273.19us   63.36us   1.56ms   92.07%
    Req/Sec    36.50k     5.03k   42.17k    91.58%
  3667586 requests in 10.10s, 1.06GB read
Requests/sec: 363127.00
Transfer/sec:    107.70MB
```

## With bpftime module, but not enabled
```console
 wrk -c 100 -t 10 http://127.0.0.1:9023
Running 10s test @ http://127.0.0.1:9023
  10 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   274.69us   64.97us   1.56ms   91.61%
    Req/Sec    36.32k     5.31k   42.17k    90.40%
  3648948 requests in 10.10s, 1.06GB read
Requests/sec: 361285.81
Transfer/sec:    107.15MB
```

## With bpftime module, enabled, url that could pass check
```console
 wrk -c 100 -t 10 http://127.0.0.1:9023
Running 10s test @ http://127.0.0.1:9023
  10 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   272.31us   69.91us   6.47ms   93.26%
    Req/Sec    36.60k     4.82k   42.30k    91.97%
  3674618 requests in 10.10s, 1.06GB read
Requests/sec: 363821.58
Transfer/sec:    107.91MB
```

## With bpftime module, enabled, url that couldn't pass check
```console
wrk -c 100 -t 10 http://127.0.0.1:9023/X
Running 10s test @ http://127.0.0.1:9023/X
  10 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   183.74us   48.64us   2.66ms   89.57%
    Req/Sec    53.89k     8.22k   77.09k    90.48%
  5404386 requests in 10.10s, 1.55GB read
  Non-2xx or 3xx responses: 5404386
Requests/sec: 535098.21
Transfer/sec:    157.17MB
```

## With lua module, url that passes the check
```console
 wrk -c 100 -t 10 http://127.0.0.1:9023
Running 10s test @ http://127.0.0.1:9023
  10 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   299.22us   68.84us   2.23ms   90.51%
    Req/Sec    33.38k     4.36k   47.39k    91.17%
  3347814 requests in 10.10s, 0.97GB read
Requests/sec: 331477.27
Transfer/sec:     98.31MB
```

## With lua module, url that couldn't pass check
```console
wrk -c 100 -t 10 http://127.0.0.1:9023/X
Running 10s test @ http://127.0.0.1:9023/X
  10 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   191.84us   52.45us   2.12ms   90.42%
    Req/Sec    51.70k     8.52k   97.58k    89.46%
  5173923 requests in 10.10s, 1.48GB read
  Non-2xx or 3xx responses: 5173923
Requests/sec: 512293.19
Transfer/sec:    150.47MB
```
## With wasm module, enabled, url that passes the check
```console
wrk -c 100 -t 10 http://127.0.0.1:9023
Running 10s test @ http://127.0.0.1:9023
  10 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   283.71us   57.23us   4.92ms   87.86%
    Req/Sec    35.06k     4.04k   52.21k    82.84%
  3515522 requests in 10.10s, 1.02GB read
Requests/sec: 348085.37
Transfer/sec:    103.24MB
```


## With wasm module, enabled, url that doesn't pass the check
```console
 wrk -c 100 -t 10 http://127.0.0.1:9023/X
Running 10s test @ http://127.0.0.1:9023/X
  10 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   181.81us   50.08us   4.71ms   93.02%
    Req/Sec    54.40k     6.92k   64.90k    92.37%
  5461028 requests in 10.10s, 1.57GB read
  Non-2xx or 3xx responses: 5461028
Requests/sec: 540700.77
Transfer/sec:    158.82MB
```
