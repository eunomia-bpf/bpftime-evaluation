# problems

## phaole

pahole: error while loading shared libraries: libdwarves_emit.so.1: cannot open shared object file: No such file or directory

fix:
```
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/
```
