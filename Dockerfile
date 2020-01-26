FROM ubuntu:18.04 as builder
RUN apt-get update \
    && apt-get -y install \
    iproute2 \
    libtbb-dev \
    libboost-all-dev 
RUN apt-get -y install \
    cmake \
    ninja-build
WORKDIR /simple_P2P
COPY create_configs.sh CMakeLists.txt ./
COPY include include
COPY src src
RUN ./create_configs.sh \
    && cd build \
    && cmake -DCMAKE_BUILD_TYPE=Debug .. \
    && make

FROM ubuntu:18.04
RUN apt-get update \
    && apt-get -y install \
    iproute2 \
    libtbb-dev \
    libboost-all-dev
WORKDIR /simple_P2P
COPY --from=builder /simple_P2P/bin/Debug/Simple_P2P .
ENTRYPOINT ./Simple_P2P \
    --my_ip $(hostname -I | awk '{print $1;}') \
    --broadcast_ip $(ip -o -f inet addr show | awk '/scope global/ {print $6}' | head -n 1)