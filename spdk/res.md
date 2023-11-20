# test res of spdk

SPDK is a high-performance user space storage application framework. It has its own tracing capability. We have tried to use bpftrace with SPDK, but the overhead of the context switches is too high to instrument high-performance IO paths. So we primarily use bpftrace for control paths, and rely on the native SPDK tracing for instrumenting high-performance IO paths.

So bpftime is something we are very interested in. Unfortunately our initial testing (seen both by me and @ksztyber) is that bpftime + bpftrace actually performed worse than bpftrace by itself. We are hoping the bpftime community could help explain what we are seeing, including anything we may have done wrong with our configuration.

## Test without probe

```console
$ sudo build/examples/bdevperf -w randread -o 4096 -q 16 -t 5 -c null.json
[2023-11-20 23:29:08.711158] Starting SPDK v24.01-pre git sha1 a26b8bd34 / DPDK 23.07.0 initialization...
[2023-11-20 23:29:08.711249] [ DPDK EAL parameters: bdevperf --no-shconf -c 0x1 --huge-unlink --no-telemetry --log-level=lib.eal:6 --log-level=lib.cryptodev:5 --log-level=user1:6 --iova-mode=pa --base-virtaddr=0x200000000000 --match-allocations --file-prefix=spdk_pid131414 ]
[2023-11-20 23:29:08.835340] app.c: 794:spdk_app_start: *NOTICE*: Total cores available: 1
[2023-11-20 23:29:08.859240] reactor.c: 937:reactor_run: *NOTICE*: Reactor started on core 0
Running I/O for 5 seconds...

                                                                                                Latency(us)
 Device Information          : runtime(s)       IOPS      MiB/s     Fail/s     TO/s    Average        min        max
 Job: null0 (Core Mask 0x1)
         null0               :       5.00 12463996.06   48687.48       0.00     0.00       1.23       0.82   21546.08
 ===================================================================================================================
 Total                       :            12463996.06   48687.48       0.00     0.00       1.23       0.82   21546.08
```

## Run with kernel bpftrace

bpftrace:

```sh
sudo bpftrace -e 'uprobe:/home/yunwei/spdk/build/examples/bdevperf:spdk_bdev_io_complete { @comple
ted = count(); }'
```

test

```console
$ sudo build/examples/bdevperf -w randread -o 4096 -q 16 -t 5 -c null.json
[2023-11-20 23:34:58.390939] Starting SPDK v24.01-pre git sha1 a26b8bd34 / DPDK 23.07.0 initialization...
[2023-11-20 23:34:58.391060] [ DPDK EAL parameters: bdevperf --no-shconf -c 0x1 --huge-unlink --no-telemetry --log-level=lib.eal:6 --log-level=lib.cryptodev:5 --log-level=user1:6 --iova-mode=pa --base-virtaddr=0x200000000000 --match-allocations --file-prefix=spdk_pid132151 ]
[2023-11-20 23:34:58.508398] app.c: 794:spdk_app_start: *NOTICE*: Total cores available: 1
[2023-11-20 23:34:58.538354] reactor.c: 937:reactor_run: *NOTICE*: Reactor started on core 0
Running I/O for 5 seconds...

                                                                                                Latency(us)
 Device Information          : runtime(s)       IOPS      MiB/s     Fail/s     TO/s    Average        min        max
 Job: null0 (Core Mask 0x1)
         null0               :       5.00  306765.63    1198.30       0.00     0.00      52.04      36.30   17057.32
 ===================================================================================================================
 Total                       :             306765.63    1198.30       0.00     0.00      52.04      36.30   17057.3
```

## With bpftime in interpreter mode

run bpftrace:

```sh
sudo LD_PRELOAD=/home/yunwei/.bpftime/libbpftime-syscall-server.so bpftrace -e 'uprobe:/home/yunwei/spdk/build/examples/bdevperf:spdk_bdev_io_complete { @completed = count(); }'
```

test:

```console
$ sudo LD_PRELOAD=/home/yunwei/.bpftime/libbpftime-agent.so build/examples/bdevperf -w randread -o 4096 -q 16 -t 5 -c null.json
[2023-11-20 23:41:23.246] [info] Entering bpftime agent
[2023-11-20 23:41:23.247] [info] Global shm constructed. shm_open_type 1 for bpftime_maps_shm
[2023-11-20 23:41:23.247] [info] Initializing agent..
[2023-11-20 23:41:23][info][134791] Executable path: /home/yunwei/spdk/build/examples/bdevperf
[2023-11-20 23:41:23][info][134791] Attached 1 uprobe programs to function 55b0fc676b30
[2023-11-20 23:41:23][info][134791] Attach successfully
[2023-11-20 23:41:23.250652] Starting SPDK v24.01-pre git sha1 a26b8bd34 / DPDK 23.07.0 initialization...
[2023-11-20 23:41:23.250756] [ DPDK EAL parameters: bdevperf --no-shconf -c 0x1 --huge-unlink --no-telemetry --log-level=lib.eal:6 --log-level=lib.cryptodev:5 --log-level=user1:6 --iova-mode=pa --base-virtaddr=0x200000000000 --match-allocations --file-prefix=spdk_pid134791 ]
[2023-11-20 23:41:23.370369] app.c: 794:spdk_app_start: *NOTICE*: Total cores available: 1
[2023-11-20 23:41:23.392207] reactor.c: 937:reactor_run: *NOTICE*: Reactor started on core 0
Running I/O for 5 seconds...

                                                                                                Latency(us)
 Device Information          : runtime(s)       IOPS      MiB/s     Fail/s     TO/s    Average        min        max
 Job: null0 (Core Mask 0x1)
         null0               :       5.00  235323.06     919.23       0.00     0.00      67.86      31.21   80797.81
 ===================================================================================================================
 Total                       :             235323.06     919.23       0.00     0.00      67.86      31.21   80797.81
INFO [134791]: Global shm destructed
```

## With bpftime in JIT mode (Compile with ubpf JIT)

run bpftrace:

```sh
sudo BPFTIME_USE_JIT=true LD_PRELOAD=/home/yunwei/.bpftime/libbpftime-syscall-server.so bpftrace -e 'uprobe:/home/yunwei/spdk/build/examples/bdevperf:spdk_bdev_io_complete { @completed = count(); }'
```

test:

```console
$ sudo LD_PRELOAD=/home/yunwei/.bpftime/libbpftime-agent.so build/examples/bdevperf -w randread -o 4096 -q 16 -t 5 -c null.json
[2023-11-20 23:45:53.176] [info] Entering bpftime agent
[2023-11-20 23:45:53.177] [info] Global shm constructed. shm_open_type 1 for bpftime_maps_shm
[2023-11-20 23:45:53.177] [info] Initializing agent..
[2023-11-20 23:45:53][info][135727] Executable path: /home/yunwei/spdk/build/examples/bdevperf
[2023-11-20 23:45:53][info][135727] Attached 1 uprobe programs to function 55b6a3b33b30
[2023-11-20 23:45:53][info][135727] Attach successfully
[2023-11-20 23:45:53.179441] Starting SPDK v24.01-pre git sha1 a26b8bd34 / DPDK 23.07.0 initialization...
[2023-11-20 23:45:53.179513] [ DPDK EAL parameters: bdevperf --no-shconf -c 0x1 --huge-unlink --no-telemetry --log-level=lib.eal:6 --log-level=lib.cryptodev:5 --log-level=user1:6 --iova-mode=pa --base-virtaddr=0x200000000000 --match-allocations --file-prefix=spdk_pid135727 ]
[2023-11-20 23:45:53.300137] app.c: 794:spdk_app_start: *NOTICE*: Total cores available: 1
[2023-11-20 23:45:53.323224] reactor.c: 937:reactor_run: *NOTICE*: Reactor started on core 0
Running I/O for 5 seconds...

                                                                                                Latency(us)
 Device Information          : runtime(s)       IOPS      MiB/s     Fail/s     TO/s    Average        min        max
 Job: null0 (Core Mask 0x1)
         null0               :       5.00 1543633.38    6029.82       0.00     0.00      10.31       4.12   52428.80
 ===================================================================================================================
 Total                       :            1543633.38    6029.82       0.00     0.00      10.31       4.12   52428.80
INFO [135727]: Global shm destructed
```
