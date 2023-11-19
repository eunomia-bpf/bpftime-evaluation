# deepflow with userspace uprobe

TODO: more complex for deepflow

with wrk:

```sh
wrk/wrk https://127.0.0.1:446/ -c 10 -t 10 -d 10
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

| Data Size | Requests/sec | Transfer/sec |
|-----------|--------------|--------------|
|10 B       |36918.99      |4.47MB        |
|1 KB       |35857.13      |39.05MB       |
|2 KB       |35574.07      |73.48MB       |
|4 KB       |28947.76      |116.86MB      |
|16 KB      |22114.53      |348.43MB      |
|128 KB     |9193.90       |1.12GB        |
|256 KB     |5765.85       |1.41GB        |
#### With kernel uprobe

| Data Size | Requests/sec | Transfer/sec |
|-----------|--------------|--------------|
|10 B       |26369.66      |3.17MB        |
|1 KB       |25261.59      |27.51MB       |
|2 KB       |23898.41      |49.37MB       |
|4 KB       |18579.09      |75.00MB       |
|16 KB      |15042.08      |237.00MB      |
|128 KB     |6496.96       |813.00MB      |
|256 KB     |4510.20       |1.10GB        |
#### With bpftime userspace uprobe (mocked hashmap (by arraymap))

- No userspace lock for shared hashmap
- With LLVM JIT
- Release mode

| Data Size | Requests/sec | Transfer/sec |
|-----------|--------------|--------------|
|10 B       |38285.68      |4.60MB        |
|1 KB       |26984.35      |29.39MB       |
|2 KB       |26051.57      |53.81MB       |
|4 KB       |27981.90      |112.96MB      |
|16 KB      |20532.01      |323.50MB      |
|128 KB     |8807.77       |1.08GB        |
|256 KB     |4599.50       |1.12GB        |

### HTTP

These tests were performed using `go-server-http/main`

#### Without trace

| Data Size | Requests/sec | Transfer/sec |
|-----------|--------------|--------------|
| 10 B      |   48174.22   |  5.79MB      |
| 1 KB      |   43417.58   |  47.29MB     |
| 2 KB      |   41130.66   |  84.96MB     |
| 4 KB      |   35208.03   |  142.13MB    |
| 16 KB     |   32904.51   |  518.43MB    |
| 128 KB    |   20155.85   |  2.46GB      |
| 256 KB    |   15352.78   |  3.75GB      |
#### With kernel uprobe

| Data Size | Requests/sec | Transfer/sec |
|-----------|--------------|--------------|
|10 B       |36592.86      |4.40MB        |
|1 KB       |35790.68      |38.98MB       |
|2 KB       |33427.74      |69.05MB       |
|4 KB       |26541.33      |107.15MB      |
|16 KB      |23743.19      |374.09MB      |
|128 KB     |14970.04      |1.83GB        |
|256 KB     |11550.26      |2.82GB        |

#### With bpftime userspace uprobe (mocked hashmap (by arraymap))

- No userspace lock for shared hashmap
- With LLVM JIT
- Release mode

| Data Size | Requests/sec | Transfer/sec |
|-----------|--------------|--------------|
|1 KB       |37977.61      |41.36MB       |
|2 KB       |37424.72      |77.31MB       |
|4 KB       |30036.29      |121.26MB      |
|16 KB      |28305.94      |445.98MB      |
|128 KB     |17180.66      |2.10GB        |
|256 KB     |12339.24      |3.01GB        |
