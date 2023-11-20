# info

I attended the bpftime talk at LPC last week. It was a very good talk and I was excited to try it with <https://github.com/spdk/spdk> - a project where I am a core maintainer.

SPDK is a high-performance user space storage application framework. It has its own tracing capability. We have tried to use bpftrace with SPDK, but the overhead of the context switches is too high to instrument high-performance IO paths. So we primarily use bpftrace for control paths, and rely on the native SPDK tracing for instrumenting high-performance IO paths.

So bpftime is something we are very interested in. Unfortunately our initial testing (seen both by me and @ksztyber) is that bpftime + bpftrace actually performed worse than bpftrace by itself. We are hoping the bpftime community could help explain what we are seeing, including anything we may have done wrong with our configuration.

Reproduction steps:

First, clone SPDK:

```sh
git clone https://github.com/spdk/spdk
git submodule update --init
```

Install dependencies:

```sh
cd spdk
scripts/pkgdep.sh
```

Build SPDK:

```sh
./configure --with-usdt
make -j
```

Paste the following into a file named null.json. This is the configuration for a null block device which will be used for the test:

```json
{
  "subsystems": [
    {
      "subsystem": "bdev",
      "config": [
        {
          "method": "bdev_null_create",
          "params": {
            "name": "null0",
            "num_blocks": 2048000,
            "block_size": 512,
            "physical_block_size": 512,
            "md_size": 0,
            "dif_type": 0,
            "dif_is_head_of_md": false,
            "uuid": "38c40683-88fe-446b-8db0-6b815396b2e7"
          }
        },
        {
          "method": "bdev_wait_for_examine"
        }
      ]
    }
  ]
}
```

Try running the SPDK bdevperf test without any probes. The test will run for 5 seconds, and then should show somewhere between 10M and 20M IOPs depending on your platform (it is 13.9M IO/s on my system):

```sh
sudo build/examples/bdevperf -w randread -o 4096 -q 16 -t 5 -c null.json
```

> May require:  `echo 1024 | sudo tee /sys/kernel/mm/hugepages/hugepages-2048kB/nr_hugepages`

Now start bpftrace by itself, and run bdevperf (note, I couldn't get the bdevperf app to terminate correctly running it as an argument to bpftrace -c):

```sh
bpftrace -e 'uprobe:/home/jimharris/git/spdk/build/examples/bdevperf:spdk_bdev_io_complete { @completed = count(); }' &
build/examples/bdevperf -w randread -o 4096 -q 16 -t 5 -c null.json
fg
^C
```

On my Xeon system, this drops the performance to about 1.6M IO/s.

Now with bpftime:

```
bpftime load bpftrace -- -e 'uprobe:/home/jimharris/git/spdk/build/examples/bdevperf:spdk_bdev_io_complete { @completed = count(); }'
# now in other terminal
bpftime start build/examples/bdevperf -- -w randread -o 4096 -q 16 -t 5 -c null.json
```

On my Xeon system, this further drops the performance to about 360K IO/s.

Please let us know if there is anything we can do to help explain this.

Thanks,

Jim Harris
