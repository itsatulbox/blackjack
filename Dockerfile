FROM debian:trixie-slim AS build
RUN apt-get update && apt-get install -y g++ cmake make && rm -rf /var/lib/apt/lists/*
WORKDIR /build
COPY CMakeLists.txt .
COPY src/ src/
RUN cmake -B out -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Release && cmake --build out

FROM debian:trixie-slim
COPY --from=build /build/out/blackjack /usr/local/bin/blackjack
