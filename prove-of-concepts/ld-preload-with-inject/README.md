# 演示如何在同一个shared object内支持LD_PRELOAD和frida gum

## 使用方式
使用CMake构建 `agent`、`injector`、`victim`这三个目标。而后`agent.so`可以被LD_PRELOAD用来覆盖puts函数，也可以被injector用来做frida remote attach。

替换后的效果如下：
```console
root@mnfe-pve:~/bpftime-poc/documents/ld-preload-with-inject# ./build/victim 
abcd
abcd
abcd
abcd
abcd
abcd
abcd
<Reversed puts: dcba> # <-- Injection happened here
<Reversed puts: dcba>
<Reversed puts: dcba>
<Reversed puts: dcba>
```
