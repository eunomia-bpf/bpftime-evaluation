# Benchmark on bpftime-nginx

```
wrk -t 10 -c 100 http://127.0.0.1:9023
```

With new bpf program

- Worker process count 10
## Without bpftime module

```console
 wrk -c 100 -t 10 http://127.0.0.1:9023/mikunot/foundexception/
Running 10s test @ http://127.0.0.1:9023/mikunot/foundexception/
  10 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   296.92us   66.63us   1.44ms   90.53%
    Req/Sec    33.53k     4.86k   54.91k    91.26%
  3359937 requests in 10.10s, 0.97GB read
Requests/sec: 332686.27
Transfer/sec:     98.67MB
```


## With bpftime module, enabled, url that could pass check
```console
wrk -c 100 -t 10 http://127.0.0.1:9023/mikunot/foundexception/
Running 10s test @ http://127.0.0.1:9023/mikunot/foundexception/
  10 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   301.57us   59.27us   1.27ms   89.52%
    Req/Sec    33.01k     3.94k   38.42k    92.48%
  3316106 requests in 10.10s, 0.96GB read
Requests/sec: 328324.01
Transfer/sec:     97.38MB
```

## With lua module, url that passes the check
```console
wrk -c 100 -t 10 http://127.0.0.1:9023/mikunot/foundexception/
Running 10s test @ http://127.0.0.1:9023/mikunot/foundexception/
  10 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   314.69us   65.68us   3.13ms   88.91%
    Req/Sec    31.64k     4.08k   51.51k    91.06%
  3170226 requests in 10.10s, 0.92GB read
Requests/sec: 313901.63
Transfer/sec:     93.10MB
```

## With wasm module, enabled, url that passes the check
```console
wrk -c 100 -t 10 http://127.0.0.1:9023/mikunot/foundexception/
Running 10s test @ http://127.0.0.1:9023/mikunot/foundexception/
  10 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   309.60us   67.52us   3.78ms   90.91%
    Req/Sec    32.19k     4.28k   37.17k    92.18%
  3234844 requests in 10.10s, 0.94GB read
Requests/sec: 320276.19
Transfer/sec:     94.99MB
```
