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
| 1 KB      | 36507.02      |  39.76MB    |
| 2 KB      |  35523.79      |  73.38MB     |
| 4 KB      |  28123.61      | 113.53MB      |
| 16 KB     | 21759.68      | 342.85MB   |
| 128 KB    |  8606.76     |  1.05GB     |
| 256 KB    |   5642.76     |  1.38GB      |
#### With kernel uprobe

| Data Size | Requests/sec | Transfer/sec |
|-----------|--------------|--------------|
|1 KB       |24650.99      |26.85MB       |
|2 KB       |24150.70      |49.89MB       |
|4 KB       |18588.55      |75.04MB       |
|16 KB      |14640.25      |230.67MB      |
|128 KB     |6296.53       |787.95MB      |
|256 KB     |3964.68       |0.97GB        |
#### With bpftime userspace uprobe (mocked hashmap (by arraymap))

- No userspace lock for shared hashmap
- With LLVM JIT
- Release mode

| Data Size | Requests/sec | Transfer/sec |
|-----------|--------------|--------------|
|1 KB       |26096.27      |28.42MB       |
|2 KB       |25639.88      |52.96MB       |
|4 KB       |18579.01      |75.00MB       |
|16 KB      |15010.58      |236.51MB      |
|128 KB     |6429.31       |804.57MB      |
|256 KB     |4161.75       |1.02GB        |

### HTTP

These tests were performed using `go-server-http/main`

#### Without trace

| Data Size | Requests/sec | Transfer/sec |
|-----------|--------------|--------------|
| 1 KB      |   43417.58   |  47.29MB     |
| 2 KB      |   41130.66   |  84.96MB     |
| 4 KB      |   35208.03   |  142.13MB    |
| 16 KB     |   35413.57   |  557.97MB    |
| 128 KB    |   20155.85   |  2.46GB      |
| 256 KB    |   15352.78   |  3.75GB      |
#### With kernel uprobe

| Data Size | Requests/sec | Transfer/sec |
|-----------|--------------|--------------|
|1 KB       |36018.41      |39.23MB       |
|2 KB       |34182.96      |70.61MB       |
|4 KB       |27675.39      |111.73MB      |
|16 KB      |23956.41      |377.46MB      |
|128 KB     |15529.68      |1.90GB        |
|256 KB     |11663.97      |2.85GB        |

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
