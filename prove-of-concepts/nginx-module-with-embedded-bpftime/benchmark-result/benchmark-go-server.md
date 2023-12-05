# Benchmark on bpftime-nginx

```
wrk -t 10 -c 100 http://127.0.
0.1:9023
```

- Worker process count 10

## Without bpftime module

```console
wrk -t 10 -c 100 http://127.0.0.1:9023
Running 10s test @ http://127.0.0.1:9023
  10 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     6.03ms    1.68ms  16.47ms   69.09%
    Req/Sec     1.66k   324.39    10.98k    98.50%
  165636 requests in 10.10s, 27.64MB read
Requests/sec:  16399.48
Transfer/sec:      2.74MB
```

## With bpftime module, but not enabled

```console
wrk -t 10 -c 100 http://127.0.0.1:9023
Running 10s test @ http://127.0.0.1:9023
  10 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     6.22ms    1.71ms  16.98ms   68.43%
    Req/Sec     1.61k   215.81     6.20k    95.81%
  160749 requests in 10.10s, 26.83MB read
Requests/sec:  15916.34
Transfer/sec:      2.66MB
```

## With bpftime module, enabled, url that could pass check

```console
wrk -t 10 -c 100 http://127.0.0.1:9023
Running 10s test @ http://127.0.0.1:9023
  10 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     6.29ms    1.74ms  15.78ms   69.26%
    Req/Sec     1.59k   214.98     5.90k    95.51%
  159038 requests in 10.10s, 26.54MB read
Requests/sec:  15748.73
Transfer/sec:      2.63MB
```

## With bpftime module, enabled, url that couldn't pass check

```console
wrk -t 10 -c 100 http://127.0.0.1:9023/X
Running 10s test @ http://127.0.0.1:9023/X
  10 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   570.69us  166.33us   4.73ms   92.26%
    Req/Sec    17.63k     3.15k   25.08k    88.89%
  1768484 requests in 10.10s, 519.45MB read
  Non-2xx or 3xx responses: 1768484
Requests/sec: 175097.53
Transfer/sec:     51.43MB
```

## With lua module, url that passes the check
```console
wrk -t 10 -c 100 http://127.0.0.1:9023
Running 10s test @ http://127.0.0.1:9023
  10 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     6.46ms    1.83ms  20.00ms   70.16%
    Req/Sec     1.55k   268.98     9.19k    98.80%
  154720 requests in 10.10s, 25.82MB read
Requests/sec:  15319.08
Transfer/sec:      2.56MB
```

## With lua module, url that couldn't passes the check

```console
wrk -t 10 -c 100 http://127.0.0.1:9023/X
Running 10s test @ http://127.0.0.1:9023/X
  10 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   607.75us  187.63us   2.52ms   92.08%
    Req/Sec    16.65k     2.96k   31.01k    90.95%
  1666954 requests in 10.10s, 489.63MB read
  Non-2xx or 3xx responses: 1666954
Requests/sec: 165055.49
Transfer/sec:     48.48MB
```

## With wasm module, enabled, url that passes the check
```console
wrk -c 100 -t 10 http://127.0.0.1:9023
Running 10s test @ http://127.0.0.1:9023
  10 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     7.98ms    7.56ms  96.74ms   93.47%
    Req/Sec     1.47k   514.01    10.31k    83.72%
  146337 requests in 10.10s, 24.42MB read
Requests/sec:  14488.57
Transfer/sec:      2.42MB
```


## With wasm module, enabled, url that doesn't pass the check
```console
wrk -c 100 -t 10 http://127.0.0.1:9023/X
Running 10s test @ http://127.0.0.1:9023/X
  10 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   672.40us  223.58us   2.85ms   92.14%
    Req/Sec    15.12k     2.83k   22.55k    89.30%
  1518057 requests in 10.10s, 445.89MB read
  Non-2xx or 3xx responses: 1518057
Requests/sec: 150304.83
Transfer/sec:     44.15MB
```

