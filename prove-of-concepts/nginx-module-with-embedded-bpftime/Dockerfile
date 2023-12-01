FROM debian:12
# Comment out the followed two lines if you are not in China

RUN rm /etc/apt/sources.list.d/*

RUN echo "deb http://mirrors.tuna.tsinghua.edu.cn/debian/ bookworm main contrib non-free non-free-firmware" > /etc/apt/sources.list \
    && echo "deb http://mirrors.tuna.tsinghua.edu.cn/debian/ bookworm-updates main contrib non-free non-free-firmware" >> /etc/apt/sources.list \
    && echo "deb http://mirrors.tuna.tsinghua.edu.cn/debian/ bookworm-backports main contrib non-free non-free-firmware" >> /etc/apt/sources.list \
    && echo "deb http://mirrors.tuna.tsinghua.edu.cn/debian-security bookworm-security main contrib non-free non-free-firmware" >> /etc/apt/sources.list

RUN apt-get update && apt-get install llvm-15-dev clang-15 cmake make libelf-dev zlib1g-dev ninja-build nginx libboost-all-dev cargo rustc curl libcrypt1 libpcre3-dev -y
RUN ln -s /usr/bin/clang-15 /usr/bin/clang && ln -s /usr/bin/clang++-15 /usr/bin/clang++ && ln -s /usr/bin/llvm-strip-15 /usr/bin/llvm-strip
ENV CC=clang-15
ENV CXX=clang++-15
ADD . /nginx-bpftime/
WORKDIR /nginx-bpftime
RUN cd bpftime && cmake -Bbuild  -DBPFTIME_ENABLE_UNIT_TESTING=0 \
    -DCMAKE_BUILD_TYPE:STRING=Release \
    -DBPFTIME_ENABLE_LTO=1 \
    -DBPFTIME_LLVM_JIT=1 \
    # -DFRIDA_DOWNLOAD_URL_PREFIX=https://ghproxy.com/ \
    -G Ninja && cmake --build build --config Release --target install
RUN rm -rf bpftime/build
RUN cmake -Bbuild  -DBPFTIME_ENABLE_UNIT_TESTING=0 \
    -DCMAKE_BUILD_TYPE:STRING=Release \
    -DBPFTIME_ENABLE_LTO=1 \
    -DBPFTIME_LLVM_JIT=1 \
    -DENABLE_LTO=1 \
    # -DFRIDA_DOWNLOAD_URL_PREFIX=https://ghproxy.com/ \
    -G Ninja
RUN cmake --build build --config Release --target nginx
# RUN cd bpftime/tools/cli-rs && cargo install --path .
RUN cd bpf-prog && make -j 32
ENV PATH=$PATH:/root/.bpftime:/root/.cargo/bin
