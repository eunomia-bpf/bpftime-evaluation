# test how syscount affect the system

The code is at bpftime/example/libbpf-tools/syscount

Test env is similar to ssl-nginx.

Run with an unexisting pid to test the effect of syscount.

```sh
$ sudo ./example/libbpf-tools/syscount/syscount -p 123
Tracing syscalls, printing top 10... Ctrl+C to quit.
```

Test nginx:

```sh
cd ssl-nginx
sudo nginx -c $(pwd)/nginx.conf -p $(pwd)
wrk/wrk https://127.0.0.1:4043/index.html -c 512 -t 4 -d 10
```

Test nginx with syscount for 20 times:

```sh
Requests/sec statistics:
Mean: 19206.49
Median: 19157.42
Stdev: 1739.92
Min: 14960.37
Max: 21936.57

Transfer/sec statistics:
Mean: 5.04
Median: 5.03
Stdev: 0.46
Min: 3.92
Max: 5.75
```

Test nginx with syscount fiilter other pid for 20 times:

```sh
Requests/sec statistics:
Mean: 18870.67
Median: 18934.76
Stdev: 2660.73
Min: 13218.06
Max: 22620.20

Transfer/sec statistics:
Mean: 4.95
Median: 4.96
Stdev: 0.70
Min: 3.47
Max: 5.93
```

Test nginx with out syscount for 20 times:

```sh
len:  20
Requests/sec statistics:
Mean: 19037.96
Median: 19335.42
Stdev: 2511.87
Min: 13510.60
Max: 23511.76

Transfer/sec statistics:
Mean: 4.99
Median: 5.07
Stdev: 0.66
Min: 3.54
Max: 6.17
```
