/**
 * Author: Greg Anderson
 * Modified by Avik De
 */
#include <iostream>
#include <chrono>

constexpr size_t ARRAY_SIZE = 128'000'000;
constexpr size_t MAX_STRIDE = 128;
constexpr size_t T = 10; // tests per stride.

float data[ARRAY_SIZE];

void warmup();
long long testStride(size_t stride);

int main()
{
    warmup();

    // No stride case
    long long noStrideTime = testStride(1);

    // 1: Test at 64 bytes
    long long strideTime = testStride(16);
    std::cout << 16 * sizeof(float) << "B stride\ttime: " << strideTime << " us\tratio: " << strideTime / static_cast<
        float>(noStrideTime) << std::endl;

    std::cout << "---" << std::endl;

    // 2: Sequential strides
    warmup();
    for (size_t stride = MAX_STRIDE; stride >= 1; stride -= 1)
    {
        auto strideTime = testStride(stride);
        std::cout << stride * sizeof(float) << "B stride\ttime: " << strideTime << " us\tratio: " << strideTime /
            static_cast<float>(noStrideTime) << std::endl;
    }
}

void warmup()
{
    float sink = 0;
    for (int i = 0; i < 10; i++)
        for (size_t j = 0; j < ARRAY_SIZE; j++)
            sink += data[j];
    std::cout << sink;
}

long long testStride(size_t stride)
{
    float sink = 0;
    /// NOTE:
    // volatile sink = 0; here ruins the Release result
    // This is because it forces sink to be stored and loaded from mem
    // instead of staying in a register.

    using clock = std::chrono::steady_clock;
    int readIndex = 0;
    auto start = clock::now();
    for (size_t i = 0; i < ARRAY_SIZE / MAX_STRIDE; i++)
    {
        sink += data[readIndex];
        readIndex += stride;
    }
    auto end = clock::now();

    // Prevent compiler from optimizing away the loop
    if (sink == -1.0f) std::cout << "";

    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}
