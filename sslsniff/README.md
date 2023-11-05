# nginx test for sslsniff

This test is to show the performance impact of kernel sslsniff and userspace sslsniff. sslsniff is a tool to intercept the ssl handshake and print the packet content of encrypted ssl handshake. The similar approach is very common in modern observability tools and security tools.

This test shoes that:

1. kernel sslsniff can significantly reduce the performance of nginx, lead to a 2x performance drop.

The test program of sslsniff is from bcc and [bpftime/example](https://github.com/eunomia-bpf/bpftime/tree/master/example/sslsniff). The userspace part modified to not print the packet content out.

## Environment

test env:

```console
$ uname -a
Linux yunwei37server 6.2.0-35-generic #35-Ubuntu SMP PREEMPT_DYNAMIC Tue Oct  3 13:14:56 UTC 2023 x86_64 x86_64 x86_64 GNU/Linux
$ nginx -v
nginx version: nginx/1.22.0 (Ubuntu)
$ ./wrk -v
wrk 4.2.0 [epoll] Copyright (C) 2012 Will Glozer
```

## Setup

start nginx server

```sh
nginx -c $(pwd)/nginx.conf -p $(pwd)
```

## Test for no effect

```console
$ ./wrk https://127.0.0.1:4043/index.html -c 100 -d 10
Running 10s test @ https://127.0.0.1:4043/index.html
  2 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     6.30ms   10.72ms 152.76ms   94.56%
    Req/Sec    10.97k     3.00k   20.62k    79.08%
  216076 requests in 10.04s, 56.67MB read
Requests/sec:  21516.79
Transfer/sec:      5.64MB
```

## Test for kernel sslsniff

in one console

```console
$ sudo ./sslsniff 
OpenSSL path: /lib/x86_64-linux-gnu/libssl.so.3
GnuTLS path: /lib/x86_64-linux-gnu/libgnutls.so.30
NSS path: /lib/x86_64-linux-gnu/libnspr4.so
FUNC         TIME(s)            COMM             PID     LEN    
lost 194 events on CPU #2
lost 61 events on CPU #3
^CTotal events: 260335 
```

This sslsniff is from bpftime/example/sslsniff/sslsniff. The userspace part modified to not print the packet content out.

In another shell

```console
$ ./wrk https://127.0.0.1:4043/index.html -c 100 -d 10
Running 10s test @ https://127.0.0.1:4043/index.html
  2 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency    20.94ms   28.31ms 289.79ms   93.24%
    Req/Sec     3.37k     1.19k    5.79k    71.35%
  65106 requests in 10.01s, 17.07MB read
Requests/sec:   6502.37
Transfer/sec:      1.71MB
```

## test for userspace sslsniff

in one console, start userspace sslsniff

```console
sudo ~/.bpftime/bpftime load example/sslsniff/sslsniff
```

in another console, restart nginx

```console
sudo ~/.bpftime/bpftime start nginx -- -c $(pwd)/nginx.conf -p $(pwd)
```
