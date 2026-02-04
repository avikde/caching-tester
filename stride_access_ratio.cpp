/**
 * Authors: Greg Anderson, Avik De
 */
#include <cstdio>
#include <chrono>

constexpr size_t ARRAY_SIZE = 64'000'000;
constexpr size_t MAX_STRIDE = 64;

float data[ARRAY_SIZE];

void warmup();
long long testStride(size_t stride);

int main()
{
    printf("STRIDED ACCESS TIME RATIO\nOutput is a CSV for easy plotting\n\n");
    warmup();

    // No stride case
    const long long noStrideTime = testStride(1);

    // // 1: Test at 64 bytes
    // long long strideTime = testStride(16);
    // printf("Ratio for 64B stride = %.3f\n---\n", strideTime / static_cast<float>(noStrideTime));

    printf("Stride, Time_us, Ratio\n");

    // 2: Sequential strides
    warmup();
    for (size_t stride = 2; stride < 64; stride += 1)
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
    constexpr size_t UNROLL = 8;
    constexpr size_t ITERATIONS = NUM_ACCESSES / UNROLL;

    size_t idx = 0;
    auto start = clock::now();

    // Each iter access data[idx .. idx + UNROLL * stride], which is 4 * UNROLL * stride = 32 * stride bytes
    // In-place increment: each access is independent (different address), so adds can pipeline
    // Trade-off: now doing load+store instead of just load
    for (size_t i = 0; i < ITERATIONS; i++)
    {
        data[idx] += 1.0f;
        data[idx + stride] += 1.0f;
        data[idx + 2 * stride] += 1.0f;
        data[idx + 3 * stride] += 1.0f;
        data[idx + 4 * stride] += 1.0f;
        data[idx + 5 * stride] += 1.0f;
        data[idx + 6 * stride] += 1.0f;
        data[idx + 7 * stride] += 1.0f;
        idx += UNROLL * stride;
    }
    auto end = clock::now();

    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}
