
To build:

```
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release # or Debug
make -j
```

Release

```
$ ./caching-tester 
no stride time: 2875 us
   stride time: 14725 us
         ratio: 5.12174
          sink: 0
```

Debug

```
$ ./caching-tester 
no stride time: 8240 us
   stride time: 8259 us
         ratio: 1.00231
          sink: 0
```
