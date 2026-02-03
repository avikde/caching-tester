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
    long long strideTime = testStride(std::atoi(argv[1]));
    
    std::cout << 16 * sizeof(float) << "B stride\ttime: " << strideTime << " us\n";
    //tratio: " << strideTime / static_cast<
      //  float>(noStrideTime) << std::endl;

    // // 1: Test at 64 bytes
    // long long strideTime = testStride(16);
    // std::cout << 16 * sizeof(float) << "B stride\ttime: " << strideTime << " us\tratio: " << strideTime / static_cast<
    //     float>(noStrideTime) << std::endl;

    // std::cout << "---" << std::endl;

    // // 2: Sequential strides
    // warmup();
    // for (size_t stride = 4; stride < 32; stride += 4)
    // {
    //     auto strideTime = testStride(stride);
    //     std::cout << stride * sizeof(float) << "B stride\ttime: " << strideTime << " us\tratio: " << strideTime /
    //         static_cast<float>(noStrideTime) << std::endl;
    // }
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
    // 8 independent accumulators to break the serial dependency chain
    // This lets the CPU issue multiple loads in parallel
    float s0 = 0, s1 = 0, s2 = 0, s3 = 0;
    float s4 = 0, s5 = 0, s6 = 0, s7 = 0;

    using clock = std::chrono::steady_clock;

    constexpr size_t NUM_ACCESSES = ARRAY_SIZE / MAX_STRIDE;
    constexpr size_t UNROLL = 8;
    size_t iterations = NUM_ACCESSES / UNROLL;

    size_t idx = 0;
    auto start = clock::now();
    for (size_t i = 0; i < iterations; i++)
    {
        s0 += data[idx];
        s1 += data[idx + stride];
        s2 += data[idx + 2 * stride];
        s3 += data[idx + 3 * stride];
        s4 += data[idx + 4 * stride];
        s5 += data[idx + 5 * stride];
        s6 += data[idx + 6 * stride];
        s7 += data[idx + 7 * stride];
        idx += UNROLL * stride;
    }
    auto end = clock::now();

    // Prevent compiler from optimizing away
    float sink = s0 + s1 + s2 + s3 + s4 + s5 + s6 + s7;
    if (sink == -1.0f) std::cout << "";

    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}
