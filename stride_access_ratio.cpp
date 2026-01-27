/**
 * Copyright (c) Greg Anderson
 */
#include <iostream>
#include <chrono>

float data[64'000'000];

void warmup();

int main()
{
    using clock = std::chrono::steady_clock;
    warmup();

    float sink = 0;

    // No stride case:
    int readIndex = 0;
    auto start1 = clock::now();
    for (int i = 0; i < 1'000'000; i++)
    {
        sink += data[readIndex];
        readIndex += 1;
    }
    auto end1 = clock::now();

    // Stride case:
    readIndex = 0;
    auto start2 = clock::now();
    for (int i = 0; i < 1'000'000; i++)
    {
        sink += data[readIndex];
        readIndex += 16;
    }
    auto end2 = clock::now();

    const auto noStrideTime = std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1).count();
    const auto strideTime = std::chrono::duration_cast<std::chrono::microseconds>(end2 - start2).count();

    std::cout << "no stride time: " << noStrideTime << " us" << std::endl;
    std::cout << "   stride time: " << strideTime << " us" << std::endl;
    std::cout << "         ratio: " << strideTime / static_cast<float>(noStrideTime) << std::endl;
    std::cout << "          sink: " << sink << std::endl;
}

void warmup()
{
    float sink = 0;
    for (int i = 0; i < 64'000'000; i++)
        sink += data[i];
}
