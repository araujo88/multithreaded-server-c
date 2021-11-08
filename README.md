# multithreaded-server-c

## Description

Multithreaded server implementation in C using websockets and POSIX threads. A single-threaded server perfomance is compared with a multi-threaded server perfomance using the `time` tool.

## Installation

Run `make all` inside the folders "multithread" and "single-thread".

## Comparing perfomance

The bash script `manyclients.bash` will run, at the same time, 50 requests from the client to the server (check `client.c` for details).

### Single-threaded server

In "single-thread" folder, run `./server`. This will make the single-threaded server start listening (port 9002). Then, at another terminal, run `time ./manyclients.bash`. This will run 50 concurrent requests to the server. At the end of the script, the following results should appear, which should differ depending on your machine perfomance:

```
real  0m2.262s
user  0m0.004s
sys   0m0.004s
```

### Multi-threaded server

For the multi-threaded server, go to the folder "multithread" and run `./server` to start the server. This will make the multi-threaded server start listening (port 9002). Then, at another terminal, run `time ./manyclients.bash`. This will run 50 concurrent requests to the server. At the end of the script, the following results should appear, which should differ depending on your machine perfomance:

```
real	0m0.018s
user  0m0.000s
sys   0m0.008s
```

As seen from the results, the multi-threaded server was around 125 times faster than the single-threaded server.
