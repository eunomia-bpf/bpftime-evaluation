```
1. 没 filter 
2. 内核 bpf ，有 filter，但不是 filter 当前的 nginx 
3. 内核 bpf，有 filter，filter 过滤当前的 nginx pid 
4. 用 bpftime，不过滤 pid，attach 到 nginx 上面
```

