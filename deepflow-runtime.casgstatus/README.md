# deepflow with userspace uprobe

## Usage
- Build and run bpftime_daemon. see https://github.com/eunomia-bpf/bpftime
- Run the go-server (./go-server/main). If not runnable, build it with `go build main.go`
- Run rust_example from https://github.com/eunomia-bpf/deepflow/tree/main/build-results . If unable to run, see https://github.com/eunomia-bpf/deepflow/blob/main/build.md for how to build it
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
root@mnfe-pve:~/deepflow# wrk -c 100 -t 10 https://127.0.0.1:446
Running 10s test @ https://127.0.0.1:446
  10 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     6.99ms   15.99ms 350.69ms   93.88%
    Req/Sec     3.26k     1.20k    7.77k    77.91%
  320042 requests in 10.10s, 39.68MB read
Requests/sec:  31688.27
Transfer/sec:      3.93MB
root@mnfe-pve:~/bpftime/example/go-trace-test# wrk -c 10 -t 10 https://127.0.0.1:446
Running 10s test @ https://127.0.0.1:446
  10 threads and 10 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   265.43us  144.65us   3.90ms   94.37%
    Req/Sec     3.84k   272.13     4.30k    92.47%
  385548 requests in 10.10s, 47.80MB read
Requests/sec:  38174.06
Transfer/sec:      4.73MB
```
#### With kernel uprobe
```console
root@mnfe-pve:~/deepflow# wrk -c 100 -t 10 https://127.0.0.1:446
Running 10s test @ https://127.0.0.1:446
  10 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency    10.60ms   14.99ms 437.61ms   89.41%
    Req/Sec     1.51k   539.99     4.39k    84.77%
  145189 requests in 10.10s, 18.00MB read
Requests/sec:  14374.98
Transfer/sec:      1.78MB
root@mnfe-pve:~/bpftime/example/go-trace-test# wrk -c 10 -t 10 https://127.0.0.1:446
Running 10s test @ https://127.0.0.1:446
  10 threads and 10 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   383.59us  206.67us   5.24ms   94.90%
    Req/Sec     2.68k   196.55     3.07k    93.45%
  268169 requests in 10.10s, 33.25MB read
Requests/sec:  26553.20
Transfer/sec:      3.29MB
```
#### With bpftime userspace uprobe (mocked hashmap (by arraymap))
- No userspace lock for shared hashmap
- With LLVM JIT
- Release mode
```console
root@mnfe-pve:~/deepflow# wrk -c 100 -t 10 https://127.0.0.1:446
Running 10s test @ https://127.0.0.1:446
  10 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     8.91ms   11.85ms 248.82ms   85.49%
    Req/Sec     1.92k   700.05     4.52k    70.82%
  189310 requests in 10.02s, 23.47MB read
Requests/sec:  18886.51
Transfer/sec:      2.34MB
root@mnfe-pve:~/bpftime/example/go-trace-test# wrk -c 10 -t 10 https://127.0.0.1:446
Running 10s test @ https://127.0.0.1:446
  10 threads and 10 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   360.93us  185.84us   4.55ms   94.51%
    Req/Sec     2.83k   214.48     3.18k    95.34%
  284074 requests in 10.10s, 35.22MB read
Requests/sec:  28126.84
Transfer/sec:      3.49MB
```
### HTTP
These tests were performed using `go-server-http/main`
#### Without trace
```console
root@mnfe-pve:~/bpftime/example/go-trace-test# wrk -c 100 -t 10 http://127.0.0.1:447
Running 10s test @ http://127.0.0.1:447
  10 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     3.97ms    5.62ms 145.05ms   88.74%
    Req/Sec     4.19k   642.85     8.18k    81.37%
  418411 requests in 10.10s, 51.87MB read
Requests/sec:  41425.58
Transfer/sec:      5.14MB
root@mnfe-pve:~/bpftime/example/go-trace-test# wrk -c 10 -t 10 http://127.0.0.1:447
Running 10s test @ http://127.0.0.1:447
  10 threads and 10 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   603.96us    4.46ms  93.68ms   98.98%
    Req/Sec     4.81k   612.91     5.55k    97.62%
  482566 requests in 10.10s, 59.83MB read
Requests/sec:  47778.24
Transfer/sec:      5.92MB
```
#### With kernel uprobe
```console
root@mnfe-pve:~/bpftime/example/go-trace-test# wrk -c 100 -t 10 http://127.0.0.1:447
Running 10s test @ http://127.0.0.1:447
  10 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency    26.01ms   87.89ms   1.04s    96.44%
    Req/Sec     1.82k   786.28     5.01k    66.91%
  173881 requests in 10.01s, 21.56MB read
Requests/sec:  17364.72
Transfer/sec:      2.15MB
root@mnfe-pve:~/bpftime/example/go-trace-test# wrk -c 10 -t 10 http://127.0.0.1:447
Running 10s test @ http://127.0.0.1:447
  10 threads and 10 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   298.07us  365.81us  15.67ms   95.81%
    Req/Sec     3.75k   183.14     4.11k    85.33%
  376415 requests in 10.10s, 46.67MB read
Requests/sec:  37270.72
Transfer/sec:      4.62MB
```
#### With bpftime userspace uprobe (mocked hashmap (by arraymap))
- No userspace lock for shared hashmap
- With LLVM JIT
- Release mode
```console
root@mnfe-pve:~/bpftime/example/go-trace-test# wrk -c 100 -t 10 http://127.0.0.1:447
Running 10s test @ http://127.0.0.1:447
  10 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     7.54ms    9.91ms 123.69ms   86.41%
    Req/Sec     2.28k   396.32     4.05k    73.80%
  227212 requests in 10.02s, 28.17MB read
Requests/sec:  22671.80
Transfer/sec:      2.81MB
root@mnfe-pve:~/bpftime/example/go-trace-test# wrk -c 10 -t 10 http://127.0.0.1:447
Running 10s test @ http://127.0.0.1:447
  10 threads and 10 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   267.16us  260.85us  11.58ms   95.30%
    Req/Sec     4.04k   184.37     4.43k    78.20%
  405645 requests in 10.10s, 50.29MB read
Requests/sec:  40165.08
Transfer/sec:      4.98MB
```
