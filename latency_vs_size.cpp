/**
 * Author: Greg Anderson
 * Modified by Avik De
 *
 * Memory latency vs working set size.
 * Based on Chips and Cheese's approach.
 *
 * Uses pointer chasing with random permutation (Sattolo's algorithm).
 * This defeats prefetchers and creates true data dependencies.
 */
#include <iostream>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <vector>

constexpr size_t MAX_SIZE = 64 * 1024 * 1024;  // 64 MB
constexpr size_t ITERATIONS = 10'000'000;
constexpr size_t CACHE_LINE = 64;

alignas(4096) char buffer[MAX_SIZE];

/**
 * Create a pointer chain using Sattolo's algorithm.
 * Each cache line points to a random other cache line, forming one big cycle.
 */
void create_pointer_chain(size_t size_bytes) {
    size_t num_lines = size_bytes / CACHE_LINE;
    void** ptrs = reinterpret_cast<void**>(buffer);

    // Initialize: each cache line points to itself (as index)
    std::vector<size_t> indices(num_lines);
    for (size_t i = 0; i < num_lines; i++) {
        indices[i] = i;
    }

    // Sattolo's algorithm: create a single cycle visiting all elements
    for (size_t i = num_lines - 1; i > 0; i--) {
        size_t j = rand() % i;  // j in [0, i-1]
        std::swap(indices[i], indices[j]);
    }

    // Set up pointers: element i points to element indices[i]
    for (size_t i = 0; i < num_lines; i++) {
        size_t next = indices[i];
        ptrs[i * CACHE_LINE / sizeof(void*)] =
            &ptrs[next * CACHE_LINE / sizeof(void*)];
    }
}

/**
 * Pointer chasing loop. Each load depends on the previous.
 */
__attribute__((noinline))
double measure_latency(size_t iterations) {
    using clock = std::chrono::high_resolution_clock;

    void** p = reinterpret_cast<void**>(buffer);

    auto start = clock::now();

    for (size_t i = 0; i < iterations; i++) {
        p = static_cast<void**>(*p);
    }

    auto end = clock::now();

    // Prevent optimization
    if (p == nullptr) std::cout << "";

    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    return static_cast<double>(ns) / iterations;
}

int main() {
    srand(42);  // Fixed seed for reproducibility

    std::cout << "Memory Latency vs Working Set Size" << std::endl;
    std::cout << "Pointer chasing with random permutation" << std::endl;
    std::cout << std::endl;
    std::cout << "   Size (KB) | Latency (ns)" << std::endl;
    std::cout << "-------------+-------------" << std::endl;

    // Test sizes from 2KB to 64MB (powers of 2)
    std::vector<size_t> sizes_kb = {
        2, 4, 8, 16, 32, 64,           // Should fit in L1 (32-48KB typical)
        128, 256, 512,                  // Should fit in L2 (256KB-1MB typical)
        1024, 2048, 4096, 8192,        // Should fit in L3 (8-32MB typical)
        16384, 32768, 65536            // Exceeds L3, goes to DRAM
    };

    for (size_t size_kb : sizes_kb) {
        size_t size_bytes = size_kb * 1024;

        // Create fresh pointer chain
        create_pointer_chain(size_bytes);

        // Warmup
        measure_latency(ITERATIONS / 10);

        // Measure
        double latency = measure_latency(ITERATIONS);

        printf("%12zu | %11.2f\n", size_kb, latency);
    }

    std::cout << std::endl;

    return 0;
}
