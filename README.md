# bpftime-evaluation

Some practical bpf performance tests

## nginx test for sslsniff

### Quantitative Analysis of Performance Drops

![image-20211013162238091](ssl-nginx/res/plot.png)

#### Kernel vs. Userspace Sniffing Impact

- **Kernel Requests Drop**:
  - The drop in requests per second due to kernel-level sniffing ranged from **29.31% to 56.56%** across different data sizes.
  - The largest impact was observed for the smallest data size (1K), where the drop was 56.56%. This suggests a significant overhead associated with kernel-level sniffing, particularly for handling smaller payloads.

- **Userspace Requests Drop**:
  - The drop in requests per second for userspace-level sniffing was lower, ranging from **5.69% to 24.31%**.
  - Again, the impact was more pronounced for smaller data sizes, with a 24.31% drop for 1K payloads, indicating better but still noticeable performance degradation.

see ssl-nginx and gpt: <https://chat.openai.com/share/3b8ded8a-2e67-4cbe-8ea7-b55dfea5191c>

## syscount for tracepint

Syscount: Tracing syscalls, printing top 10...

The similar approach is used widely in observation daemons.

The test result shows that:

- Syscount to trace all results: Mean: 32053.66
- Syscount filter other pid: Mean: 35716.34
- No effect: Mean: 39335.21

If we want to use the tracepoint to trace system calls for a target process, it will also affect other process, bring about 10% unnecessary overhead. This is because the syscall tracepoint is a global tracepoint, and it will be triggered for all processes. If you want to trace a specific process, you need to filter the pid in the kernel.

What's more, if you want to filter the pid in userspace, it will bring 20% overhead.
