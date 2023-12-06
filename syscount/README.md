# test how syscount affect the system

Syscount: Tracing syscalls, printing top 10...

The similar approach is used widely in observation daemons.

The test result shows that:

- Syscount to trace all results: Mean: 32053.66
- Syscount filter other pid: Mean: 35716.34
- No effect: Mean: 39335.21

If we want to use the tracepoint to trace system calls for a target process, it will also affect other process, bring about 10% unnecessary overhead. This is because the syscall tracepoint is a global tracepoint, and it will be triggered for all processes. If you want to trace a specific process, you need to filter the pid in the kernel.

What's more, if you want to filter the pid in userspace, it will bring 20% overhead.

The code is at bpftime/example/libbpf-tools/syscount

Test env is similar to ssl-nginx.

Test nginx:

```sh
sudo nginx -c $(pwd)/nginx.conf -p $(pwd)
make
```

```


## userspace syscount:


len:  20
Requests/sec statistics:
Mean: 8593.33
Median: 8311.02
Stdev: 1081.26
Min: 7094.49
Max: 11368.49

Transfer/sec statistics:
Mean: 2.18
Median: 2.10
Stdev: 0.27
Min: 1.80
Max: 2.88


## no syscount:


len:  20
Requests/sec statistics:
Mean: 12292.24
Median: 12796.33
Stdev: 2367.95
Min: 7911.74
Max: 15582.43

Transfer/sec statistics:
Mean: 3.12
Median: 3.25
Stdev: 0.60
Min: 2.01
Max: 3.95


## kernel target filter nginx:


len:  10
Requests/sec statistics:
Mean: 9668.96
Median: 9698.80
Stdev: 910.97
Min: 8072.23
Max: 11575.63

Transfer/sec statistics:
Mean: 2.45
Median: 2.46
Stdev: 0.23
Min: 2.05
Max: 2.94


## kernel target filter other process:


len:  10
Requests/sec statistics:
Mean: 10413.39
Median: 10899.91
Stdev: 1631.72
Min: 6812.87
Max: 11909.52

Transfer/sec statistics:
Mean: 2.64
Median: 2.76
Stdev: 0.41
Min: 1.73
Max: 3.02


## userspace target filter other process:


len:  20
Requests/sec statistics:
Mean: 12302.01
Median: 12706.75
Stdev: 1628.12
Min: 8677.74
Max: 14526.87

Transfer/sec statistics:
Mean: 3.12
Median: 3.23
Stdev: 0.41
Min: 2.20
Max: 3.69
root@mnfe-pve:~/bpftime-evaluation/syscount# 

## userspace syscount:


len:  20
Requests/sec statistics:
Mean: 8593.33
Median: 8311.02
Stdev: 1081.26
Min: 7094.49
Max: 11368.49

Transfer/sec statistics:
Mean: 2.18
Median: 2.10
Stdev: 0.27
Min: 1.80
Max: 2.88


## no syscount:


len:  20
Requests/sec statistics:
Mean: 12292.24
Median: 12796.33
Stdev: 2367.95
Min: 7911.74
Max: 15582.43

Transfer/sec statistics:
Mean: 3.12
Median: 3.25
Stdev: 0.60
Min: 2.01
Max: 3.95


## kernel target filter nginx:


len:  10
Requests/sec statistics:
Mean: 9668.96
Median: 9698.80
Stdev: 910.97
Min: 8072.23
Max: 11575.63

Transfer/sec statistics:
Mean: 2.45
Median: 2.46
Stdev: 0.23
Min: 2.05
Max: 2.94


## kernel target filter other process:


len:  10
Requests/sec statistics:
Mean: 10413.39
Median: 10899.91
Stdev: 1631.72
Min: 6812.87
Max: 11909.52

Transfer/sec statistics:
Mean: 2.64
Median: 2.76
Stdev: 0.41
Min: 1.73
Max: 3.02


## userspace target filter other process:


len:  20
Requests/sec statistics:
Mean: 12302.01
Median: 12706.75
Stdev: 1628.12
Min: 8677.74
Max: 14526.87

Transfer/sec statistics:
Mean: 3.12
Median: 3.23
Stdev: 0.41
Min: 2.20
Max: 3.69
root@mnfe-pve:~/bpftime-evaluation/syscount# 
```

## Test nginx with syscount for 20 times

Run with syscount.

```sh
$ sudo ./example/libbpf-tools/syscount/syscount
Tracing syscalls, printing top 10... Ctrl+C to quit.
```

res:

```sh
Requests/sec statistics:
Mean: 32053.66
Median: 32288.62
Stdev: 3092.01
Min: 27080.95
Max: 37208.48

Transfer/sec statistics:
Mean: 9.97
Median: 10.04
Stdev: 0.96
Min: 8.42
Max: 11.57
```

## Test nginx with syscount fiilter other pid for 20 times

Run with an unexisting pid to test the effect of syscount.

```sh
$ sudo ./example/libbpf-tools/syscount/syscount -p 123
Tracing syscalls, printing top 10... Ctrl+C to quit.
```

res:

```sh
Requests/sec statistics:
Mean: 35716.34
Median: 35967.76
Stdev: 2479.69
Min: 31142.76
Max: 39226.01

Transfer/sec statistics:
Mean: 11.10
Median: 11.18
Stdev: 0.77
Min: 9.68
Max: 12.20
```

## Test nginx without syscount for 20 times

```sh
len:  20
Requests/sec statistics:
Mean: 39335.21
Median: 39495.93
Stdev: 2765.97
Min: 31005.75
Max: 42489.23

Transfer/sec statistics:
Mean: 12.23
Median: 12.28
Stdev: 0.86
Min: 9.64
Max: 13.21
```
