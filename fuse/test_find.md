# test find

test server:

```sh
LD_PRELOAD=/root/zys/bpftime/build/runtime/syscall-server/libbpftime-syscall-server.so ./fs-filter-cache/fs-cache
```

test find:


```sh
time AGENT_SO=/root/zys/bpftime/build/runtime/agent/libbpftime-agent.so LD_PRELOAD=/root/zys/bpftime/build/runtime/agent-transformer/libbpftime-agent-transformer.so find /root/bpftime-evaluation/fuse/data 2>/dev/null 1>/dev/null
```

res:

```

```