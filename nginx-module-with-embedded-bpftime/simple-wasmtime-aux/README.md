# How to compile the wasm program
```console
clang -O2 handler.c -target wasm32  --no-standard-libraries -Wl,--export-all -Wl,--no-entry -o handler.wasm
```
