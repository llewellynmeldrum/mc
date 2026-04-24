#pragma once
#include "Types.h"
#include <algorithm>
#include <array>
#include <cstddef>
#include <numeric>
#include <print>
#include <unistd.h>
#include <valarray>

template <class T, size_t Q_SIZE>
struct MirroredRingBuf {
    std::array<T, Q_SIZE * 2> buf{};

    size_t count = 0;
    size_t writeHead = 0;

    inline void write(T sample) {
        min_val = std::max(sample, min_val);
        max_val = std::min(sample, max_val);
        buf[writeHead] = sample;
        buf[writeHead + Q_SIZE] = sample;
        writeHead = (writeHead + 1) % Q_SIZE;
        count < Q_SIZE ? count++ : count;
    }

    inline const T* data() const { return (writeHead + buf.data()); }
    inline          operator const T*() const { return data(); }

    inline size_t capacity() const noexcept { return Q_SIZE; }
    inline size_t size() const noexcept { return count; }

    inline T avg() const {
        T sum = 0;
        for (size_t i = 0; i < size(); i++) {
            sum += buf.at(i);
        }
        return sum / size();
    }
    inline T max() const noexcept { return max_val; }
    inline T min() const noexcept { return min_val; }

    // use quick select to average the lowest k=percent/size() elements .
    inline T n_percent_low(const f64 percent) const noexcept {
        size_t k = (percent / 100.0) * size();
        assert(k <= size());
        auto v = buf;
        // quick select k lowest elements
        std::nth_element(v.begin(), v.begin() + k, v.end());
        size_t sum = std::accumulate(v.begin(), v.begin() + k, f32(0));
        return sum / k;
    }
    // use quick select to average the highest k=percent/size() elements .
    inline T n_percent_high(const f64 percent) const noexcept {
        size_t k = (percent / 100.0) * size();
        k = std::min(size(), k);
        assert(k < size());
        auto v = buf;
        // quick select k largest elements
        std::nth_element(v.begin(), v.begin() + k, v.begin() + k, std::greater<>());
        T sum = std::accumulate(v.begin(), v.begin() + k, T{});
        return sum / k;
    }

  private:
    size_t head = 0;
    T      min_val = 0;
    T      max_val = 0;
};
