/**
 * Author: Greg Anderson
 * Modified by Avik De
 */
#include <iostream>
#include <chrono>

constexpr size_t ARRAY_SIZE = 64'000'000;
constexpr size_t MAX_STRIDE = 64;

float data[ARRAY_SIZE];

void warmup();
long long testStride(size_t stride);

int main(int argc, char **argv)
{
    warmup();

    // No stride case
    const long long noStrideTime = testStride(1);

    // 2: Sequential strides
    for (size_t stride = 2; stride < 32; stride++)
    {
        auto strideTime = testStride(stride);
        std::cout << stride * sizeof(float) << "B stride\ttime: " << strideTime << " us\tratio: " << strideTime /
            static_cast<float>(noStrideTime) << std::endl;
    }
    return 0;
}

void warmup()
{
    float sink = 0;
    for (int i = 0; i < 10; i++)
        for (size_t j = 0; j < ARRAY_SIZE; j++)
            sink += data[j];

    // Prevent compiler from optimizing away the loop
    if (sink == -1.0f) std::cout << "";
}

long long testStride(size_t stride)
{
    using clock = std::chrono::steady_clock;

    // Keep # accesses consistent as stride changes
    constexpr size_t NUM_ACCESSES = ARRAY_SIZE / MAX_STRIDE;
    constexpr size_t UNROLL = 8;
    constexpr size_t ITERATIONS = NUM_ACCESSES / UNROLL;

    // 8 independent accumulators to break the data dependency between loop iterations
    // This lets the CPU issue multiple loads in parallel
    float accumm[UNROLL] = {0};

    size_t idx = 0;
    auto start = clock::now();
    for (size_t i = 0; i < ITERATIONS; i++)
    {
        // Access data with index strides
        for (size_t j = 0; j < UNROLL; j++)
        {
            accumm[j] += data[idx + j * stride];
        }
        idx += UNROLL * stride;
    }
    auto end = clock::now();

    // Prevent compiler from optimizing away
    float sink = 0;
    for (size_t j = 0; j < UNROLL; j++)
    {
        sink += accumm[j];
    }
    if (sink == -1.0f) std::cout << "";

    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}
