/**
 * Tests various stride sizes to detect cache line boundaries.
 * Performance cliff indicates cache line size.
 */
#include <iostream>
#include <chrono>
#include <iomanip>

constexpr size_t ARRAY_SIZE = 64'000'000;
constexpr int ITERATIONS = 1'000'000;

float data[ARRAY_SIZE];

void warmup()
{
    float sink = 0;
    for (size_t i = 0; i < ARRAY_SIZE; i++)
        sink += data[i];
}

long long test_stride(int stride)
{
    using clock = std::chrono::steady_clock;

    float sink = 0;
    int readIndex = 0;

    auto start = clock::now();
    for (int i = 0; i < ITERATIONS; i++)
    {
        sink += data[readIndex];
        readIndex += stride;
        if (readIndex >= ARRAY_SIZE)
            readIndex = 0;
    }
    auto end = clock::now();

    // Prevent optimization of sink
    if (sink == -1.0f) std::cout << "";

    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

int main()
{
    warmup();

    std::cout << "Testing cache line boundaries...\n";
    std::cout << "Stride (floats) | Stride (bytes) | Time (us) | Relative\n";
    std::cout << "----------------+----------------+-----------+---------\n";

    long long baseline = test_stride(1);

    // Test powers of 2 from 1 to 128 floats
    for (int stride = 1; stride <= 128; stride *= 2)
    {
        long long time = test_stride(stride);
        float relative = static_cast<float>(time) / baseline;

        std::cout << std::setw(15) << stride << " | "
                  << std::setw(14) << (stride * 4) << " | "
                  << std::setw(9) << time << " | "
                  << std::fixed << std::setprecision(2) << relative << "\n";
    }

    std::cout << "\n";

    // Test around typical cache line size (16 floats = 64 bytes)
    std::cout << "Fine-grained test around 64 bytes:\n";
    std::cout << "Stride (floats) | Stride (bytes) | Time (us) | Relative\n";
    std::cout << "----------------+----------------+-----------+---------\n";

    for (int stride = 8; stride <= 24; stride++)
    {
        long long time = test_stride(stride);
        float relative = static_cast<float>(time) / baseline;

        std::cout << std::setw(15) << stride << " | "
                  << std::setw(14) << (stride * 4) << " | "
                  << std::setw(9) << time << " | "
                  << std::fixed << std::setprecision(2) << relative << "\n";
    }

    return 0;
}
