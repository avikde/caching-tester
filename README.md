
To build:

```shell
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release # or Debug
make -j
```

# Stride access ratio

This program is designed to show the effects of strided access. There are 3 different mode in which to run:

1. Accumulate into sink
2. Unroll the loop while accumulating in order to avoid pipeline stalls from the RAW data dependency on the `sink` variable
3. In-place Read-Modify-Write of the array

What we expect to see:

Effectively, as we access data, the processor can load a segment from main memory into cache, in blocks. As the stride increases, to visit the same number of elements, a larger number of blocks will need to be loaded. So, if the memory movement dominates, we expect to initially see a linear rise in time as stride increases as the number of cache lines touched rises. It should eventually plateau after some point after each access is already hitting a unique cache line, though exactly when this happens should depend on the associativity of the cache and L1 cache size.

To run, you can use the numeric value of the mode:

`./stride_access_ratio <mode>`

The default mode is `3 = Read-Modify-Write`.

# Latency vs. size

Estimate cache sizes, roughly based on information from Chips and Cheese implementation, but simplified.

