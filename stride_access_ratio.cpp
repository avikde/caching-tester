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

    // 2: Sequential strides
    warmup();
    for (size_t stride = MAX_STRIDE; stride >= 1; stride -= 1)
    {
        double sum = 0.0;
        for (size_t i = 0; i < T; ++i)
            sum += testStride(stride);

        double avgStrideTime = sum / T;

        std::cout << stride * sizeof(float) << "B stride\ttime: " << avgStrideTime
            << " us\tratio: " << avgStrideTime / static_cast<double>(noStrideTime)
            << std::endl;
    }
}

void warmup()
{
    float sink = 0;
    for (size_t i = 0; i < ARRAY_SIZE; i++)
        sink += data[i];
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
