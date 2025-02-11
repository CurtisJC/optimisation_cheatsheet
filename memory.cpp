///////////////////////////////////////////////////////////////////////////////////////////////////
// Memory Alignment and Cache Optimization
// Proper alignment and contiguous memory access help reduce cache misses and improve performance.
// Cache-friendly access patterns can lead to significant speedups when working with large datasets.
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <cstddef> //size_t
#include <new>
#include <vector>

alignas(64) std::vector<float> aligned_data(1000); // Aligned for SIMD-friendly access

void cache_friendly_access(std::vector<std::vector<int>> &matrix) {
    for (std::size_t i = 0; i < matrix.size(); ++i) {
        for (std::size_t j = 0; j < matrix[i].size(); ++j) {
            matrix[i][j] += 1; // Row-major order access improves cache locality.
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// Software Prefetching
// Prefetching loads data into cache before it is needed, reducing memory latency.
// This is particularly useful in loops that process large data structures.
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <cstddef> //size_t
#include <immintrin.h>

void software_prefetch(float *data, std::size_t N) {
    for (std::size_t i = 0; i < N; i += 4) {
        _mm_prefetch((char *)&data[i + 16], _MM_HINT_T0); // Prefetch next cache line
        data[i] += 1.0f; // Operate on prefetched data
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
