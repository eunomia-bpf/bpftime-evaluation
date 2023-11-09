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
root@mnfe-pve:~/go-https-server-test# wrk https://127.0.0.1:446  -c 100 -t 10
Running 10s test @ https://127.0.0.1:446
  10 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     3.75ms    5.01ms 112.19ms   87.93%
    Req/Sec     3.98k   774.36     7.12k    87.81%
  393188 requests in 10.10s, 48.75MB read
Requests/sec:  38927.86
Transfer/sec:      4.83MB
```
### With kernel uprobe
```console
root@mnfe-pve:~/go-https-server-test# wrk https://127.0.0.1:446  -c 100 -t 10
Running 10s test @ https://127.0.0.1:446
  10 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency    10.29ms   14.05ms 200.14ms   87.09%
    Req/Sec     1.71k   495.75     4.45k    76.32%
  168179 requests in 10.02s, 20.85MB read
Requests/sec:  16777.05
Transfer/sec:      2.08MB
```
### With bpftime userspace uprobe
- No userspace lock for shared hashmap
- With LLVM JIT
- Release mode
```console
root@mnfe-pve:~/go-https-server-test# wrk https://127.0.0.1:446  -c 100 -t 10
Running 10s test @ https://127.0.0.1:446
  10 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency    10.41ms   16.19ms 350.39ms   88.89%
    Req/Sec     1.79k   520.84     3.57k    71.57%
  175589 requests in 10.04s, 21.77MB read
Requests/sec:  17496.67
Transfer/sec:      2.17MB
```
