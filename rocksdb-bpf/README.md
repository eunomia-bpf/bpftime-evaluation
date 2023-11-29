# rockdb

benchmark command:

```
./db_bench --benchmarks=fillrandom --num=10000000 --value_size=1024 --db=your_running_db_folder
```

new command:

## kernel uprobe

cannot change exec flow of userspace function

```console
root@banana:~/zys/bpftime-evaluation/AisLSM/build# ./db_bench --benchmarks="fillrandom,stats" --num=10000000 --value_size=8192 --db=your_running_db_folder --level0_file_num_compaction_trigger=2 --level0_slowdown_writes_trigger=40 --level0_stop_writes_trigger=60  
Set seed to 1701285792561867 because --seed was 0
Initializing RocksDB Options from the specified file
Initializing RocksDB Options from command-line flags
Integrated BlobDB: blob cache disabled
RocksDB:    version 7.10.0
Date:       Wed Nov 29 19:23:12 2023
CPU:        48 * Intel(R) Xeon(R) Gold 5418Y
CPUCache:   46080 KB
Keys:       16 bytes each (+ 0 bytes user-defined timestamp)
Values:     8192 bytes each (4096 bytes after compression)
Entries:    10000000
Prefix:    0 bytes
Keys per prefix:    0
RawSize:    78277.6 MB (estimated)
FileSize:   39215.1 MB (estimated)
Write rate: 0 bytes/second
Read rate: 0 ops/second
Compression: Snappy
Compression sampling rate: 0
Memtablerep: SkipListFactory
Perf Level: 1
------------------------------------------------
Initializing RocksDB Options from the specified file
Initializing RocksDB Options from command-line flags
Integrated BlobDB: blob cache disabled
DB path: [your_running_db_folder]
fillrandom   :      72.623 micros/op 13769 ops/sec 726.232 seconds 10000000 operations;  107.8 MB/s


** Compaction Stats [default] **
Level    Files   Size     Score Read(GB)  Rn(GB) Rnp1(GB) Write(GB) Wnew(GB) Moved(GB) W-Amp Rd(MB/s) Wr(MB/s) Comp(sec) CompMergeCPU(sec) Comp(cnt) Avg(sec) KeyIn KeyDrop Rblob(GB) Wblob(GB)
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  L0     32/0    1.10 GB  16.0      0.0     0.0      0.0      42.6     42.6       0.0   1.0      0.0    303.3    143.82            137.60      1234    0.117       0      0       0.0       0.0
  L1     95/1    5.94 GB  23.5    182.7    41.5    141.2     177.9     36.8       0.0   4.3    355.0    345.8    526.94            526.38        40   13.174     42M  1107K       0.0       0.0
  L2    461/4   23.76 GB   9.4     57.5    25.8     31.7      50.4     18.7       5.0   2.0    357.4    313.5    164.71            164.46       354    0.465     13M  1655K       0.0       0.0
 Sum    588/5   30.81 GB   0.0    240.2    67.3    172.9     271.0     98.1       5.0   6.4    294.3    332.1    835.47            828.45      1628    0.513     56M  2762K       0.0       0.0
 Int      0/0    0.00 KB   0.0     37.4     9.7     27.8      40.2     12.4       0.0   7.6    275.9    296.0    138.98            138.16       228    0.610   8779K   606K       0.0       0.0

** Compaction Stats [default] **
Priority    Files   Size     Score Read(GB)  Rn(GB) Rnp1(GB) Write(GB) Wnew(GB) Moved(GB) W-Amp Rd(MB/s) Wr(MB/s) Comp(sec) CompMergeCPU(sec) Comp(cnt) Avg(sec) KeyIn KeyDrop Rblob(GB) Wblob(GB)
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 Low      0/0    0.00 KB   0.0    240.2    67.3    172.9     228.4     55.5       0.0   0.0    355.5    338.1    691.65            690.84       394    1.755     56M  2762K       0.0       0.0
High      0/0    0.00 KB   0.0      0.0     0.0      0.0      42.6     42.6       0.0   0.0      0.0    303.3    143.82            137.60      1234    0.117       0      0       0.0       0.0

Blob file count: 0, total size: 0.0 GB, garbage size: 0.0 GB, space amp: 0.0

Uptime(secs): 726.2 total, 123.2 interval
Flush(GB): cumulative 42.596, interval 5.316
AddFile(GB): cumulative 0.000, interval 0.000
AddFile(Total Files): cumulative 0, interval 0
AddFile(L0 Files): cumulative 0, interval 0
AddFile(Keys): cumulative 0, interval 0
Cumulative compaction: 270.97 GB write, 382.06 MB/s write, 240.15 GB read, 338.62 MB/s read, 835.5 seconds
Interval compaction: 40.17 GB write, 333.83 MB/s write, 37.44 GB read, 311.15 MB/s read, 139.0 seconds
Stalls(count): 311 level0_slowdown, 97 level0_slowdown_with_compaction, 0 level0_numfiles, 0 level0_numfiles_with_compaction, 0 stop for pending_compaction_bytes, 0 slowdown for pending_compaction_bytes, 924 memtable_compaction, 0 memtable_slowdown, interval 183 total count
Block cache LRUCache@0x55555564f910#535028 capacity: 8.00 MB usage: 336.11 KB table_size: 256 occupancy: 352568 collections: 2 last_copies: 0 last_secs: 8.2e-05 secs_since: 123
Block cache entry stats(count,size,portion): Misc(43,340.61 KB,4.15783%)

** File Read Latency Histogram By Level [default] **

** DB Stats **
Uptime(secs): 726.2 total, 123.2 interval
Cumulative writes: 10M writes, 10M keys, 10M commit groups, 1.0 writes per commit group, ingest: 76.59 GB, 108.00 MB/s
Cumulative WAL: 10M writes, 0 syncs, 10000000.00 writes per sync, written: 76.59 GB, 108.00 MB/s
Cumulative stall: 00:09:50.644 H:M:S, 81.3 percent
Interval writes: 1254K writes, 1254K keys, 1254K commit groups, 1.0 writes per commit group, ingest: 9840.88 MB, 79.86 MB/s
Interval WAL: 1254K writes, 0 syncs, 1254731.00 writes per sync, written: 9.61 GB, 79.86 MB/s
Interval stall: 00:01:45.095 H:M:S, 85.3 percent

clear!
```

enable uprobe and kernel work together:

```console
# LD_PRELOAD=/root/zys/bpftime/build/runtime/agent/libbpftime-agent.so ./db_bench --benchmarks="fillrandom,stats" --num=10000000 --value_size=8192 --db=your_running_db_folder --level0_file_num_compaction_trigger=2 --level0_slowdown_writes_trigger=40 --level0_stop_writes_trigger=60  
[2023-11-29 23:13:24.790] [info] [agent.cpp:66] Entering bpftime agent
[2023-11-29 23:13:24.791] [info] [bpftime_shm_internal.cpp:597] Global shm constructed. shm_open_type 1 for bpftime_maps_shm
[2023-11-29 23:13:24.791] [info] [agent.cpp:81] Initializing agent..
[2023-11-29 23:13:24][info][694467] Executable path: /root/zys/bpftime-evaluation/AisLSM/build/db_bench
[2023-11-29 23:13:24][info][694467] Attach successfully
Set seed to 1701299604796503 because --seed was 0
Initializing RocksDB Options from the specified file
Initializing RocksDB Options from command-line flags
Integrated BlobDB: blob cache disabled
RocksDB:    version 7.10.0
Date:       Wed Nov 29 23:13:24 2023
CPU:        48 * Intel(R) Xeon(R) Gold 5418Y
CPUCache:   46080 KB
Keys:       16 bytes each (+ 0 bytes user-defined timestamp)
Values:     8192 bytes each (4096 bytes after compression)
Entries:    10000000
Prefix:    0 bytes
Keys per prefix:    0
RawSize:    78277.6 MB (estimated)
FileSize:   39215.1 MB (estimated)
Write rate: 0 bytes/second
Read rate: 0 ops/second
Compression: Snappy
Compression sampling rate: 0
Memtablerep: SkipListFactory
Perf Level: 1
------------------------------------------------
Initializing RocksDB Options from the specified file
Initializing RocksDB Options from command-line flags
Integrated BlobDB: blob cache disabled
DB path: [your_running_db_folder]
fillrandom   :      78.684 micros/op 12709 ops/sec 786.842 seconds 10000000 operations;   99.5 MB/s


** Compaction Stats [default] **
Level    Files   Size     Score Read(GB)  Rn(GB) Rnp1(GB) Write(GB) Wnew(GB) Moved(GB) W-Amp Rd(MB/s) Wr(MB/s) Comp(sec) CompMergeCPU(sec) Comp(cnt) Avg(sec) KeyIn KeyDrop Rblob(GB) Wblob(GB)
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  L0     16/0   565.60 MB   8.0      0.0     0.0      0.0      42.6     42.6       0.0   1.0      0.0    274.9    158.66            152.52      1234    0.129       0      0       0.0       0.0
  L1     99/1    6.16 GB  24.5    179.9    42.0    137.8     175.2     37.4       0.0   4.2    315.5    307.3    583.70            583.26        40   14.592     42M  1098K       0.0       0.0
  L2    461/1   23.89 GB   9.5     59.9    27.2     32.7      52.6     19.9       4.0   1.9    316.4    277.8    193.95            193.76       377    0.514     14M  1713K       0.0       0.0
 Sum    576/2   30.60 GB   0.0    239.8    69.2    170.5     270.4     99.8       4.0   6.3    262.2    295.7    936.30            929.53      1651    0.567     56M  2811K       0.0       0.0
 Int      0/0    0.00 KB   0.0     57.1    14.8     42.3      60.7     18.4       0.0   8.0    269.7    286.9    216.77            215.52       339    0.639     13M   924K       0.0       0.0

** Compaction Stats [default] **
Priority    Files   Size     Score Read(GB)  Rn(GB) Rnp1(GB) Write(GB) Wnew(GB) Moved(GB) W-Amp Rd(MB/s) Wr(MB/s) Comp(sec) CompMergeCPU(sec) Comp(cnt) Avg(sec) KeyIn KeyDrop Rblob(GB) Wblob(GB)
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 Low      0/0    0.00 KB   0.0    239.8    69.2    170.5     227.8     57.2       0.0   0.0    315.7    299.9    777.65            777.01       417    1.865     56M  2811K       0.0       0.0
High      0/0    0.00 KB   0.0      0.0     0.0      0.0      42.6     42.6       0.0   0.0      0.0    274.9    158.66            152.52      1234    0.129       0      0       0.0       0.0

Blob file count: 0, total size: 0.0 GB, garbage size: 0.0 GB, space amp: 0.0

Uptime(secs): 786.8 total, 183.8 interval
Flush(GB): cumulative 42.596, interval 7.594
AddFile(GB): cumulative 0.000, interval 0.000
AddFile(Total Files): cumulative 0, interval 0
AddFile(L0 Files): cumulative 0, interval 0
AddFile(Keys): cumulative 0, interval 0
Cumulative compaction: 270.38 GB write, 351.87 MB/s write, 239.77 GB read, 312.04 MB/s read, 936.3 seconds
Interval compaction: 60.73 GB write, 338.28 MB/s write, 57.08 GB read, 317.95 MB/s read, 216.8 seconds
Stalls(count): 295 level0_slowdown, 106 level0_slowdown_with_compaction, 0 level0_numfiles, 0 level0_numfiles_with_compaction, 0 stop for pending_compaction_bytes, 0 slowdown for pending_compaction_bytes, 965 memtable_compaction, 0 memtable_slowdown, interval 245 total count
Block cache LRUCache@0x55555563e6f0#694467 capacity: 8.00 MB usage: 336.09 KB table_size: 256 occupancy: 352552 collections: 2 last_copies: 0 last_secs: 9.6e-05 secs_since: 183
Block cache entry stats(count,size,portion): Misc(42,332.48 KB,4.05865%)

** File Read Latency Histogram By Level [default] **

** DB Stats **
Uptime(secs): 786.8 total, 183.8 interval
Cumulative writes: 10M writes, 10M keys, 10M commit groups, 1.0 writes per commit group, ingest: 76.59 GB, 99.68 MB/s
Cumulative WAL: 10M writes, 0 syncs, 10000000.00 writes per sync, written: 76.59 GB, 99.68 MB/s
Cumulative stall: 00:10:46.047 H:M:S, 82.1 percent
Interval writes: 1785K writes, 1785K keys, 1785K commit groups, 1.0 writes per commit group, ingest: 14007.42 MB, 76.19 MB/s
Interval WAL: 1785K writes, 0 syncs, 1785973.00 writes per sync, written: 13.68 GB, 76.19 MB/s
Interval stall: 00:02:38.171 H:M:S, 86.0 percent

INFO [694467]: Global shm destructed
clear!
root@banana:~/zys/bpftime-evaluation/AisLSM/build# 
```

override all:

```console
 LD_PRELOAD=/root/zys/bpftime/build/runtime/agent/libbpftime-agent.so ./db_bench --benchmarks="fillrandom,stats" --num=10000000 --value_size=8192 --db=your_running_db_folder --level0_file_num_compaction_trigger=2 --level0_slowdown_writes_trigger=40 --level0_stop_writes_trigger=60  
[2023-11-29 23:30:28.570] [info] [agent.cpp:66] Entering bpftime agent
[2023-11-29 23:30:28.572] [info] [bpftime_shm_internal.cpp:597] Global shm constructed. shm_open_type 1 for bpftime_maps_shm
[2023-11-29 23:30:28.572] [info] [agent.cpp:81] Initializing agent..
[2023-11-29 23:30:28][info][708665] Executable path: /root/zys/bpftime-evaluation/AisLSM/build/db_bench
[2023-11-29 23:30:28][info][708665] Attach successfully
Set seed to 1701300628577395 because --seed was 0
Initializing RocksDB Options from the specified file
Initializing RocksDB Options from command-line flags
Integrated BlobDB: blob cache disabled
RocksDB:    version 7.10.0
Date:       Wed Nov 29 23:30:29 2023
CPU:        48 * Intel(R) Xeon(R) Gold 5418Y
CPUCache:   46080 KB
Keys:       16 bytes each (+ 0 bytes user-defined timestamp)
Values:     8192 bytes each (4096 bytes after compression)
Entries:    10000000
Prefix:    0 bytes
Keys per prefix:    0
RawSize:    78277.6 MB (estimated)
FileSize:   39215.1 MB (estimated)
Write rate: 0 bytes/second
Read rate: 0 ops/second
Compression: Snappy
Compression sampling rate: 0
Memtablerep: SkipListFactory
Perf Level: 1
------------------------------------------------
Initializing RocksDB Options from the specified file
Initializing RocksDB Options from command-line flags
Integrated BlobDB: blob cache disabled
DB path: [your_running_db_folder]
fillrandom   :      78.074 micros/op 12808 ops/sec 780.741 seconds 10000000 operations;  100.3 MB/s


** Compaction Stats [default] **
Level    Files   Size     Score Read(GB)  Rn(GB) Rnp1(GB) Write(GB) Wnew(GB) Moved(GB) W-Amp Rd(MB/s) Wr(MB/s) Comp(sec) CompMergeCPU(sec) Comp(cnt) Avg(sec) KeyIn KeyDrop Rblob(GB) Wblob(GB)
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  L0     41/0    1.42 GB  20.5      0.0     0.0      0.0      42.6     42.6       0.0   1.0      0.0    283.6    153.93            147.73      1235    0.125       0      0       0.0       0.0
  L1     89/1    5.60 GB  22.1    179.7    41.2    138.5     175.0     36.6       0.0   4.2    319.5    311.2    575.81            575.43        39   14.764     42M  1091K       0.0       0.0
  L2    469/4   23.82 GB   9.5     58.3    26.5     31.8      51.2     19.3       4.5   1.9    305.3    267.9    195.55            195.39       371    0.527     13M  1673K       0.0       0.0
 Sum    599/5   30.83 GB   0.0    238.0    67.7    170.3     268.8     98.5       4.5   6.3    263.4    297.5    925.29            918.55      1645    0.562     55M  2765K       0.0       0.0
 Int      0/0    0.00 KB   0.0     58.2    15.4     42.8      62.5     19.7       0.0   7.4    270.7    291.0    219.98            218.59       378    0.582     13M   970K       0.0       0.0

** Compaction Stats [default] **
Priority    Files   Size     Score Read(GB)  Rn(GB) Rnp1(GB) Write(GB) Wnew(GB) Moved(GB) W-Amp Rd(MB/s) Wr(MB/s) Comp(sec) CompMergeCPU(sec) Comp(cnt) Avg(sec) KeyIn KeyDrop Rblob(GB) Wblob(GB)
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 Low      0/0    0.00 KB   0.0    238.0    67.7    170.3     226.2     55.9       0.0   0.0    315.9    300.3    771.36            770.82       410    1.881     55M  2765K       0.0       0.0
High      0/0    0.00 KB   0.0      0.0     0.0      0.0      42.6     42.6       0.0   0.0      0.0    283.6    153.93            147.73      1235    0.125       0      0       0.0       0.0

Blob file count: 0, total size: 0.0 GB, garbage size: 0.0 GB, space amp: 0.0

Uptime(secs): 780.7 total, 177.7 interval
Flush(GB): cumulative 42.629, interval 8.491
AddFile(GB): cumulative 0.000, interval 0.000
AddFile(Total Files): cumulative 0, interval 0
AddFile(L0 Files): cumulative 0, interval 0
AddFile(Keys): cumulative 0, interval 0
Cumulative compaction: 268.81 GB write, 352.56 MB/s write, 237.97 GB read, 312.12 MB/s read, 925.3 seconds
Interval compaction: 62.51 GB write, 360.14 MB/s write, 58.16 GB read, 335.07 MB/s read, 220.0 seconds
Stalls(count): 316 level0_slowdown, 100 level0_slowdown_with_compaction, 0 level0_numfiles, 0 level0_numfiles_with_compaction, 0 stop for pending_compaction_bytes, 0 slowdown for pending_compaction_bytes, 902 memtable_compaction, 0 memtable_slowdown, interval 286 total count
Block cache LRUCache@0x55555563e6f0#708665 capacity: 8.00 MB usage: 344.30 KB table_size: 256 occupancy: 352568 collections: 2 last_copies: 0 last_secs: 0.000102 secs_since: 177
Block cache entry stats(count,size,portion): Misc(43,340.61 KB,4.15783%)

** File Read Latency Histogram By Level [default] **

** DB Stats **
Uptime(secs): 780.7 total, 177.7 interval
Cumulative writes: 10M writes, 10M keys, 10M commit groups, 1.0 writes per commit group, ingest: 76.59 GB, 100.46 MB/s
Cumulative WAL: 10M writes, 0 syncs, 10000000.00 writes per sync, written: 76.59 GB, 100.46 MB/s
Cumulative stall: 00:10:38.310 H:M:S, 81.8 percent
Interval writes: 1985K writes, 1985K keys, 1985K commit groups, 1.0 writes per commit group, ingest: 15571.06 MB, 87.61 MB/s
Interval WAL: 1985K writes, 0 syncs, 1985340.00 writes per sync, written: 15.21 GB, 87.61 MB/s
Interval stall: 00:02:29.716 H:M:S, 84.2 percent

INFO [708665]: Global shm destructed
clear!
```