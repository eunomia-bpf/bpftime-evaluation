# Build deepflow v6.1.1 that can run on kernel 6.2.16

## Clone this repo
```console
git clone --recursive  https://github.com/eunomia-bpf/deepflow
```

This repo is large. Make sure you have a stable connection to github

## Checkout version v6.1.1

```console
cd deepflow
git checkout 49ade95a4d6cb7b1a0ccbb74730ff29b0e8fcc07
```

## Update corresponding submodules

```console
git submodule update
```

## Use deepflow's building image to build it

- Firstly, create a container and get into it

```
docker run --privileged -e HTTP_PROXY=http://172.17.0.1:2345 -e HTTPS_PROXY=http://172.17.0.1:2345 -it -v $(pwd):/deepflow -v ~/.cargo:/usr/local/cargo hub.deepflow.yunshan.net/public/rust-build bash
```

- Then, in the container:
```console
cd deepflow/agent
cargo build --release
```
Also make sure you have a stable connection to github.
After building, you can find the build results under agent/target/release

- Finally, we build the example program
```console
make -C /deepflow/agent/src/ebpf rust-sample
```
You can then find the example program at $(pwd)/agent/src/ebpf/samples/rust/target/release/rust_sample

## Dependencies of deepflow-agent
If you would like to directly execute deepflow-agent, at least one of the dependencies (libpcap) should be built manually:
```console
git clone --recursive https://github.com/the-tcpdump-group/libpcap
cd libpcap
cmake .
make -j && make install
ln -s  /usr/local/lib/libpcap.so.1 /lib/libpcap.so.1 
```
# More
This version(v6.1.1) of deepflow was tested to be run on kernel 6.2.16.

Build results on my machine could be found at the `build-results` folder.
