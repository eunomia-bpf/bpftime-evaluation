# deepflow with userspace uprobe

## Usage
- Build and run bpftime_daemon. see https://github.com/eunomia-bpf/bpftime
- Run the go-server (./go-server/main). If not runnable, build it with `go build main.go`
- Run rust_example from https://github.com/eunomia-bpf/deepflow/tree/main/build-results . If unable to run, see https://github.com/eunomia-bpf/deepflow/blob/main/build.md for how to build it
- Use attach mode to run bpftime agent. `bpftime attach PID`. PID could be retrived from `ps -aux | grep main`

## Test examples


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
### Without trace
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
```
### With kernel uprobe
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
```
### With bpftime userspace uprobe (mocked hashmap (by arraymap))
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
```
