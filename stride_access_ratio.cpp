/**
 * Authors: Greg Anderson, Avik De
 */
#include <cstdio>
#include <chrono>

constexpr size_t ARRAY_SIZE = 64'000'000;
constexpr size_t MAX_STRIDE = 64;

alignas(64) float data[ARRAY_SIZE];

void warmup();
long long testStride(size_t stride);

int main()
{
    printf("STRIDED ACCESS TIME RATIO\nOutput is a CSV for easy plotting\n\n");
    warmup();

    // No stride case
    const long long noStrideTime = testStride(1);

    printf("Stride, Time_us, Ratio\n");

    // Sequential strides: 
    warmup();
    for (size_t stride = 2; stride < 32; stride += 1)
    {
        auto strideTime = testStride(stride);
        printf("%lu, %lld, %.3f\n", stride, strideTime,
            strideTime / static_cast<float>(noStrideTime));
    }
}

void warmup()
{
    float sink = 0;
    for (int i = 0; i < 10; i++)
        for (size_t j = 0; j < ARRAY_SIZE; j++)
            sink += data[j];

    // Prevent compiler from optimizing away the loop
    if (sink == -1.0f) printf("\n");
}

long long testStride(size_t stride)
{
    using clock = std::chrono::steady_clock;

    // Keep # accesses consistent as stride changes
    constexpr size_t NUM_ACCESSES = ARRAY_SIZE / MAX_STRIDE;

    size_t idx = 0;
    auto start = clock::now();

    // In-place increment: each access is independent (different address), so adds can pipeline
    for (size_t i = 0; i < NUM_ACCESSES; i++)
    {
        data[idx] += 1.0f;
        idx += stride;
    }
    auto end = clock::now();

    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}
