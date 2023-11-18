# deepflow with userspace uprobe

TODO: more complex for deepflow

with wrk:

```sh
wrk/wrk https://127.0.0.1:4043/index.html -c 512 -t 4 -d 10
```

| Data Size | Requests/sec | Transfer/sec |
|-----------|--------------|--------------|
| 1 KB      |              |              |
| 2 KB      |              |              |
| 4 KB      |              |              |
| 16 KB     |              |              |
| 128 KB    |              |              |
| 256 KB    |              |              |

Test with 4 different types:

1. deepflow with partly userspace uprobe in bpftime
2. deepflow with kernel uprobe, totally running in kernel
3. deepflow without enable uprobe, only kprobe or syscall tracepoints, sockets
4. without deepflow

You should test it with two types of server:

1. Golang server with https enabled, use goroutine to handle requests
2. Golang server with only http enabled, use goroutine to handle requests

## Usage

- Build and run bpftime_daemon. see <https://github.com/eunomia-bpf/bpftime>
- Run the go-server (./go-server/main). If not runnable, build it with `go build main.go`
- Run rust_example from <https://github.com/eunomia-bpf/deepflow/tree/main/build-results> . If unable to run, see <https://github.com/eunomia-bpf/deepflow/blob/main/build.md> for how to build it
- Use attach mode to run bpftime agent. `bpftime attach PID`. PID could be retrived from `ps -aux | grep main`

## Test examples (https)

On my machine:

```console
root@mnfe-pve 
------------- 
OS: Proxmox VE 8.0.4 x86_64 
Host: PowerEdge R720 
Kernel: 6.2.16-19-pve 
Shell: bash 5.2.15 
Terminal: node 
CPU: Intel Xeon E5-2697 v2 (48) @ 3.500GHz 
GPU: NVIDIA Tesla P40 
GPU: AMD ATI Radeon HD 7470/8470 / R5 235/310 OEM 
Memory: 81639MiB / 257870MiB 
```

### HTTPS

These tests were performed using `go-server/main`

#### Without trace

```console
root@mnfe-pve:~/deepflow# wrk -c 10 -t 10 https://127.0.0.1:446
Running 10s test @ https://127.0.0.1:446
  10 threads and 10 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   283.60us  215.40us   5.49ms   93.34%
    Req/Sec     3.81k   335.28     4.29k    96.14%
  383164 requests in 10.10s, 47.50MB read
Requests/sec:  37935.70
Transfer/sec:      4.70MB
```

#### With kernel uprobe

```console
root@mnfe-pve:~/deepflow# wrk -c 10 -t 10 https://127.0.0.1:446
Running 10s test @ https://127.0.0.1:446
  10 threads and 10 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   342.61us  212.46us   3.74ms   89.09%
    Req/Sec     3.05k   591.47     4.95k    77.30%
  306242 requests in 10.10s, 37.97MB read
Requests/sec:  30321.11
Transfer/sec:      3.76MB
```

#### With bpftime userspace uprobe (mocked hashmap (by arraymap))

- No userspace lock for shared hashmap
- With LLVM JIT
- Release mode

```console
root@mnfe-pve:~/deepflow# wrk -c 10 -t 10 https://127.0.0.1:446
Running 10s test @ https://127.0.0.1:446
  10 threads and 10 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   273.69us  173.21us   4.93ms   93.94%
    Req/Sec     3.75k     0.88k    6.60k    67.47%
  373118 requests in 10.10s, 46.26MB read
Requests/sec:  36942.72
Transfer/sec:      4.58MB
```

### HTTP

These tests were performed using `go-server-http/main`

#### Without trace

```console
root@mnfe-pve:~/deepflow# wrk -c 10 -t 10 http://127.0.0.1:447
Running 10s test @ http://127.0.0.1:447
  10 threads and 10 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   709.25us    5.17ms 102.39ms   98.67%
    Req/Sec     4.91k   698.19     5.74k    96.43%
  492576 requests in 10.10s, 61.07MB read
Requests/sec:  48773.46
Transfer/sec:      6.05MB
```

#### With kernel uprobe

```console
root@mnfe-pve:~/deepflow# wrk -c 10 -t 10 http://127.0.0.1:447
Running 10s test @ http://127.0.0.1:447
  10 threads and 10 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   252.71us  186.02us   5.23ms   94.17%
    Req/Sec     4.25k   554.85     6.31k    66.50%
  426355 requests in 10.10s, 52.86MB read
Requests/sec:  42214.27
Transfer/sec:      5.23MB
```

#### With bpftime userspace uprobe (mocked hashmap (by arraymap))

- No userspace lock for shared hashmap
- With LLVM JIT
- Release mode

```console
root@mnfe-pve:~/deepflow# wrk -c 10 -t 10 http://127.0.0.1:447
Running 10s test @ http://127.0.0.1:447
  10 threads and 10 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   217.71us  148.94us   4.05ms   94.43%
    Req/Sec     4.82k     1.00k    8.04k    62.57%
  484277 requests in 10.10s, 60.04MB read
Requests/sec:  47949.59
Transfer/sec:      5.94MB
```
