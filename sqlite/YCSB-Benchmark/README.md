
To Test sqlite latency:
```
1. To the sqlite directory
    mkdir build
    ./configure --prefix=/path /to /build
    make
    make install

2. Prepare for YCSB Benchmark
    git clone YCSB-C repository:
        git clone https://github.com/ls4154/YCSB-cpp
        cd /path_to_ycsb
        git submodule update --init


3. Modify Makefile of YCSB. 
        For your convienice, sqlite directory contains YCSB folder which modifies Makefile
        You only need to do the below 2 things:

            1. change CXXFLAGS to your sqlite source code directory as the template
            2. change LDFLAGS  to your sqlite build/lib directory  (build is the directory you have mkdired before)
        Modify the db.path of sqlite.properties file in the YCSB sqlite directory
    Attention: 
        If the modification of Makefile is not correct, YCSB will use the default sqlite in the linux system to run rather than io_uring sqlite
  
4. Make YCSB
    make BIND_SQLITE=1
```

5. Running

First load data with sqlite:
```
./ycsb -load -db sqlite -P workloads/workloada -P sqlite/sqlite.properties -s
```

Second Run workload A with sqlite:
```
./ycsb -run -db sqlite -P workloads/workloada -P sqlite/sqlite.properties -s
```

you could refer test.sh  for all workloads (LOAD, A, B, C, D, E,F)
