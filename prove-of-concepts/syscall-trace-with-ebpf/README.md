# 兼容kernel态syscall trace ebpf程序的用户态syscall trace实现

## 核心思路

类似于[zpoline](https://github.com/yasukata/zpoline)，重写text段中的代码，将`syscall`/`sysenter`替换为`jmp *%rax`，而后在对应地址处填充适当的代码以进入hook函数。

## 用到的可执行文件

### `injector.cpp`/ `injector`

一个注入器，负责将特定的动态库注入到指定进程中，以实现让该动态库重写text段。

### `agent.cpp`/`agent.so`

被注入的第一个动态库。这个动态库负责重写被注入进程的text段中的相应指令。重写完成后，其会使用`dlmopen`加载另一个动态库`hooker.so`。

### `hooker.cpp`/`hooker.so`

被`agent.so`使用dlmopen所加载的动态库。由于使用dlmopen加载，所以其使用的libc中的syscall并未被重写。这个动态库负责加载编译ebpf程序，并且在syscall hooker被调用时，触发指定的ebpf程序回调并进行处理。

### `victim.cpp`/`victim`

一个用于演示`opensnoop`的被注入的程序。其死循环调用open、write、close系统调用。

### `test_driver.cpp`/`test_driver`

演示程序，会驱动上面所有程序来完成一次正常的注入过程。

## 性能测试

### 不使用任何trace手段

使用Release构建本项目中的victim目标，而后执行该目标。等待一定时间后（比如几十秒）后按Ctrl+C，会结束进程并且输出open调用的平均时间。

```console
^Copen/openat performed 22 times, total time usage 1871918 ns, average 85087.181818 ns
```

### 使用kernel提供的syscall trace

- `git clone --recursive https://github.com/iovisor/bcc`
- `cd bcc/libbpf-tools && make opensnoop`
- `./opensnoop`
- 其他与上一部分一致。

```console
^Copen/openat performed 28 times, total time usage 3188091 ns, average 113860.392857 ns
```

### 使用本项目提供的userspace syscall trace

- 使用Release profile构建本项目中的test_driver目标并运行。

```console
open/openat performed 26 times, total time usage 2901481 ns, average 111595.423077 ns
```
