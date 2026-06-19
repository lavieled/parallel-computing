#ifndef PREFIX_SUM_2D_IMPL_H
#define PREFIX_SUM_2D_IMPL_H

#include "prefix_sum_2D.h"

#include <algorithm>

namespace {

// 1D prefix sum along one row
void rowPrefixSum(TableAbs& table, unsigned long long N, unsigned long long row) {
    for (unsigned long long j = 1; j < N; ++j) {
        table[row * N + j] += table[row * N + j - 1];
    }
}

// 1D prefix sum along one column
void colPrefixSum(TableAbs& table, unsigned long long N, unsigned long long col) {
    for (unsigned long long i = 1; i < N; ++i) {
        table[i * N + col] += table[(i - 1) * N + col];
    }
}

}  // namespace

void prefixSum_serial(TableAbs& table, unsigned long long N) {
    for (unsigned long long i = 0; i < N; ++i) {
        rowPrefixSum(table, N, i);
    }
    for (unsigned long long j = 0; j < N; ++j) {
        colPrefixSum(table, N, j); // rows first, then cols
    }
}

void prefixSum_threads(TableAbs& table, unsigned long long N, int numThreads) {
    int threads = std::max(1, numThreads);

    std::vector<std::thread> workers;
    workers.reserve(static_cast<std::size_t>(threads));

    // pass 1: rows in parallel
    for (int t = 0; t < threads; ++t) {
        workers.emplace_back([&table, N, threads, t]() {
            for (unsigned long long i = t; i < N; i += static_cast<unsigned long long>(threads)) {
                rowPrefixSum(table, N, i);
            }
        });
    }
    for (auto& worker : workers) {
        worker.join();
    }

    workers.clear();
    // pass 2: cols in parallel
    for (int t = 0; t < threads; ++t) {
        workers.emplace_back([&table, N, threads, t]() {
            for (unsigned long long j = t; j < N; j += static_cast<unsigned long long>(threads)) {
                colPrefixSum(table, N, j);
            }
        });
    }
    for (auto& worker : workers) {
        worker.join();
    }
}

void prefixSum_tbb(TableAbs& table, unsigned long long N) {
    tbb::parallel_for(
        tbb::blocked_range<unsigned long long>(0, N),
        [&table, N](const tbb::blocked_range<unsigned long long>& range) {
            for (unsigned long long i = range.begin(); i != range.end(); ++i) {
                rowPrefixSum(table, N, i);
            }
        });

    tbb::parallel_for(
        tbb::blocked_range<unsigned long long>(0, N),
        [&table, N](const tbb::blocked_range<unsigned long long>& range) {
            for (unsigned long long j = range.begin(); j != range.end(); ++j) {
                colPrefixSum(table, N, j);
            }
        });
}

#endif
