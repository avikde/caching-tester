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
    // constexpr size_t UNROLL = 8; // 2)
    // constexpr size_t ITERATIONS = NUM_ACCESSES / UNROLL; // 2)

    // // Explicit scalar accumulators - compiler keeps these in registers
    // // Prevent RAW hazard with a single accumulator
    // float s0 = 0, s1 = 0, s2 = 0, s3 = 0; // 2)
    // float s4 = 0, s5 = 0, s6 = 0, s7 = 0; // 2)
    // float sink = 0; // 1) or 2)
    size_t idx = 0;
    
    auto start = clock::now();

    // // 1) Accumulate into sink
    // for (size_t i = 0; i < NUM_ACCESSES; i++)
    // {
    //     sink += data[idx];
    //     idx += stride;
    // }

    // // 2) Unroll; separate accumulators
    // // Each iter access data[idx .. idx + UNROLL * stride], which is 4 * UNROLL * stride = 32 * stride bytes
    // for (size_t i = 0; i < ITERATIONS; i++)
    // {
    //     s0 += data[idx];
    //     s1 += data[idx + stride];
    //     s2 += data[idx + 2 * stride];
    //     s3 += data[idx + 3 * stride];
    //     s4 += data[idx + 4 * stride];
    //     s5 += data[idx + 5 * stride];
    //     s6 += data[idx + 6 * stride];
    //     s7 += data[idx + 7 * stride];
    //     idx += UNROLL * stride;
    // }
    
    // 3) In-place increment: each access is independent (different address), so adds can pipeline
    for (size_t i = 0; i < NUM_ACCESSES; i++)
    {
        data[idx] += 1.0f;
        idx += stride;
    }
    auto end = clock::now();
    
    // // Prevent compiler from optimizing away the loop
    // sink = s0 + s1 + s2 + s3 + s4 + s5 + s6 + s7; // 2)
    // if (sink == -1.0f) printf("\n"); // 1) or 2)

    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}
