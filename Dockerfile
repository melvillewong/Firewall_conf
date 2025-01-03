FROM ubuntu:latest
ENV DEBIAN_FRONTEND=noninteractive

RUN apt update && \
    apt install -y gcc make valgrind cppcheck libcriterion-dev && \
    apt clean && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . /app

RUN make static-check
RUN make
