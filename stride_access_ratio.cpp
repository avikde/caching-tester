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

    // 1: Test at 64 bytes
    long long strideTime = testStride(16);
    printf("Ratio = %.3f\n---\n", strideTime / static_cast<float>(noStrideTime));

    printf("Stride_bytes, Time_us, Ratio\n");

    // 2: Sequential strides
    warmup();
    for (size_t stride = 2; stride < 32; stride += 2)
    {
        auto strideTime = testStride(stride);
        printf("%lu, %lld, %.3f\n", stride * sizeof(float), strideTime,
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

    // Explicit scalar accumulators - compiler keeps these in registers
    // (arrays often get spilled to stack, adding unwanted memory traffic)
    float s0 = 0, s1 = 0, s2 = 0, s3 = 0;
    float s4 = 0, s5 = 0, s6 = 0, s7 = 0;

    size_t idx = 0;
    auto start = clock::now();
    for (size_t i = 0; i < ITERATIONS; i++)
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
    if (sink == -1.0f) printf("\n");

    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}
