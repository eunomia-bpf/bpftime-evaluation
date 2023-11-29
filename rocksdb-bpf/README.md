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