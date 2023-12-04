FROM debian:12

# Comment out the followed two lines if you are not in China

RUN rm /etc/apt/sources.list.d/*

RUN echo "deb http://mirrors.tuna.tsinghua.edu.cn/debian/ bookworm main contrib non-free non-free-firmware" > /etc/apt/sources.list \
    && echo "deb http://mirrors.tuna.tsinghua.edu.cn/debian/ bookworm-updates main contrib non-free non-free-firmware" >> /etc/apt/sources.list \
    && echo "deb http://mirrors.tuna.tsinghua.edu.cn/debian/ bookworm-backports main contrib non-free non-free-firmware" >> /etc/apt/sources.list \
    && echo "deb http://mirrors.tuna.tsinghua.edu.cn/debian-security bookworm-security main contrib non-free non-free-firmware" >> /etc/apt/sources.list

RUN apt-get update && apt-get upgrade -y
RUN apt-get install -y gcc g++ cmake make zlib1g-dev libelf-dev nginx wget llvm-15-dev llvm-15 wrk
RUN mkdir /root/.bpftime
COPY /assets/* /root/.bpftime/
# COPY /sslsniff /root/sslsniff
RUN ln -s /root/.bpftime /root/assets
COPY /deepflow-runtime.casgstatus /root/deepflow-runtime.casgstatus
ENV PATH="$PATH:/root/.bpftime"
WORKDIR /root
