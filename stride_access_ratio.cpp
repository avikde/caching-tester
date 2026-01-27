/**
 * Copyright (c) Greg Anderson
 */
#include <iostream>
#include <chrono>

float data[64'000'000];

void warmup();
long long testStride(int stride);

int main()
{
    // data.resize(64'000'000);
    warmup();

    // No stride case:
    auto noStrideTime = testStride(1);
    auto strideTime = testStride(16);

    std::cout << "no stride time: " << noStrideTime << " us" << std::endl;
    std::cout << "   stride time: " << strideTime << " us" << std::endl;
    std::cout << "         ratio: " << strideTime / static_cast<float>(noStrideTime) << std::endl;
}

void warmup()
{
    float sink = 0;
    for (int i = 0; i < 64'000'000; i++)
        sink += data[i];
}

long long testStride(int stride)
{
    float sink = 0;
    /// NOTE:
    // volatile sink = 0; here ruins the Release result
    // This is because it forces sink to be stored and loaded from mem
    // instead of staying in a register.

    using clock = std::chrono::steady_clock;
    int readIndex = 0;
    auto start = clock::now();
    for (size_t i = 0; i < 1'000'000; i++)
    {
        sink += data[readIndex];
        readIndex += stride;
    }
    auto end = clock::now();

    // Prevent compiler from optimizing away the loop
    if (sink == -1.0f) std::cout << "";

    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

