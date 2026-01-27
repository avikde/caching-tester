
To build:

```shell
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release # or Debug
make -j
```

## Results

Release

```shell
$ ./caching-tester 
no stride time: 2875 us
   stride time: 14725 us
         ratio: 5.12174
          sink: 0
```

Debug

```shell
$ ./caching-tester 
no stride time: 8240 us
   stride time: 8259 us
         ratio: 1.00231
          sink: 0
```

## Understanding the ratio

### Release mode

Build and output the object file with debug symbols:
```shell
g++ -std=c++17 -O3 -g -c stride_access_ratio.cpp -o stride_access_ratio.o
```
With the object file:

```shell
objdump -d -C -S stride_access_ratio.o | grep -A 50 "testStride"
```

```shell
    for (size_t i = 0; i < ARRAY_SIZE / MAX_STRIDE; i++)
  c8:   movslq %eax,%rcx              # Sign-extend readIndex to 64-bit
  cb:   add    $0x1,%eax              # readIndex += 1 (stride hardcoded!)
  ce:   addss  (%rsi,%rcx,4),%xmm0   # sink += data[readIndex]
  d3:   sub    $0x1,%rdx              # i--
  d7:   jne    c8                     # Loop if i != 0
```

Slight difference for `stride != 0`, only in the modification of readIndex - everything else is the same.

```shell
 153:   add    %edi,%eax              # readIndex += stride (from %edi)
```

Observe:

- No vectorization: Compiler uses scalar addss (single float) instead of vectorized SIMD instructions
- Memory access pattern: (%rsi,%rcx,4) computes data[readIndex] where %rsi is the base address
- The difference in performance comes entirely from the memory access pattern hitting cache differently, not from differences in instruction count
- Register usage: `%xmm0` holds sink, `%eax` holds readIndex, `%rdx` is the loop counter
- No loop unrolling: the loop is probably too large to do this.

The assembly is remarkably similar between stride=1 and stride=16 - the 5x performance difference is purely from cache misses.

### Compare to Debug mode

Replace the `-O3` with `-O0`, and repeat the objdump. The code breakdown reveals:

```shell
    for (size_t i = 0; i < ARRAY_SIZE / MAX_STRIDE; i++)
 239:   movq   $0x0,-0x10(%rbp)        # i = 0 (initialize loop counter)
 241:   jmp    27d                      # Jump to loop condition check

    # LOOP BODY START
 243:   mov    -0x34(%rbp),%eax        # Load readIndex from stack
 246:   cltq                            # Sign-extend to 64-bit
 248:   lea    0x0(,%rax,4),%rdx       # rdx = readIndex * 4 (offset in bytes)
 250:   lea    0x0(%rip),%rax          # Load data array base address
 257:   movss  (%rdx,%rax,1),%xmm0    # Load data[readIndex] into xmm0
 25c:   movss  -0x38(%rbp),%xmm1      # Load sink from stack into xmm1
 261:   addss  %xmm1,%xmm0            # sink + data[readIndex]
 265:   movss  %xmm0,-0x38(%rbp)      # Store result back to sink on stack

        readIndex += stride;
 26a:   mov    -0x48(%rbp),%rax        # Load stride from stack
 26e:   mov    %eax,%edx                # Copy to edx
 270:   mov    -0x34(%rbp),%eax        # Load readIndex from stack
 273:   add    %edx,%eax                # readIndex += stride
 275:   mov    %eax,-0x34(%rbp)        # Store readIndex back to stack

    # LOOP INCREMENT AND CHECK
 278:   addq   $0x1,-0x10(%rbp)        # i++
 27d:   cmpq   $0xf423f,-0x10(%rbp)    # Compare i < 1,000,000
 285:   jbe    243                      # Jump back if i <= limit
```

So, the comparison reveals:

|Aspect|**-O3 (Optimized)**|**-O0 (Debug)**|
|---|---|---|
|**Instructions per iteration**|5 instructions|17 instructions|
|**Register usage**|`sink` stays in `%xmm0`|`sink` loaded from memory every iteration|
|**Variable storage**|Registers only|All variables on stack (-0x38, -0x34, -0x48, -0x10)|
|**Memory operations per iteration**|1 read (`data[readIndex]`)|6 memory ops (load sink, load readIndex, load stride, store sink, store readIndex, check i)|

The -O0 version performs **6 memory operations per iteration**, likely completely overwhelming the cache behavior we're trying to measure. The memory accesses add so much overhead that accessing data sequentially or with stride becomes irrelevant - both are slow due to the loads and stores.

