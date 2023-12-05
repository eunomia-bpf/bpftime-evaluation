
To Test sqlite latency:
```
1. To the sqlite directory
    mkdir build
    cd build
    ../configure --prefix=/path /to /build
    Add -luring to CFLAGS in the Makefile that you generated in the above step
    make
    make install

2. Prepare for YCSB Benchmark
    For your convienice, sqlite directory contains YCSB folder

    (Optional) Or you could directly git clone from origiin YCSB-C repository:
        git clone https://github.com/ls4154/YCSB-cpp
        cd /path_to_ycsb
        git submodule update --init


3. Modify Makefile of YCSB. 
        
        Path in YCSB's Makefile should be specified:

            1. change CXXFLAGS to your sqlite source code directory as the template
            2. change LDFLAGS  to your sqlite build/lib directory  (build is the directory you have mkdired before)

        Then you could use this Makefile to replace the origin Makefile in your cloned YCSB folder. 


    Attention: 
        If the modification of Makefile is not correct, YCSB will use the default sqlite in the linux system to run rather than io_uring sqlite
  
4. Make YCSB
    make BIND_SQLITE=1

    To test different versions of sqlite, change the path in Makefile. and run make clean
```

5. Running

Modify the db.path of sqlite.properties file in the YCSB sqlite directory

Load data with sqlite:
```
./ycsb -load -db sqlite -P workloads/workloada -P sqlite/sqlite.properties -s
```

Run workload A with sqlite:
```
./ycsb -run -db sqlite -P workloads/workloada -P sqlite/sqlite.properties -s
```

To run workload B, C .... just change the workloads/workload? to B, C....


Pass additional properties:
Take rocksdb for an example
```
./ycsb -load -db sqlite -P workloads/workloadb -P rocksdb/rocksdb.properties \
  -p sqlite.cache_size=134217728 -s
```
