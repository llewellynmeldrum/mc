#pragma once
#include <algorithm>
#include <array>
#include <cstddef>
#include <numeric>
#include <print>
#include <unistd.h>
#include <valarray>
#include "cpp23_ranges.hpp"

#include "Types.h"
#include "Assertion.hpp"
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
    T stddev() const {
        auto mean = avg();
        f32 sum{};
        for (size_t i = 0; i<count; i++){
            auto dev = buf[i] - mean;
            auto dev2 = dev*dev;
            sum+=dev2;
        }
        return std::sqrt(sum/count);
    }
    inline T max() const noexcept { return max_val; }
    inline T min() const noexcept { return min_val; }

    // use quick select to average the lowest k=percent/size() elements .
    inline T n_percent_low(const f64 percent) const noexcept {
        size_t k = (percent / 100.0) * size();
        assert(k <= size());
        auto v = buf;
        // quick select k lowest elements
        ranges::nth_element(v, v.begin() + k);
        size_t sum = std::accumulate(v.begin(), v.begin() + k, f32(0));
        return sum / k;
    }
    // use quick select to average the highest k=percent/size() elements .
    inline T n_percent_high(const f64 percent) const noexcept {
        size_t k = (percent / 100.0) * size();
        // TODO: broken
        k = std::min(size(), k);
        auto v = buf;
        // quick select k largest elements
        std::nth_element(v.begin(), v.begin() + k, v.begin() + k, std::greater<>());
        T sum = std::accumulate(v.begin(), v.begin() + k, T{});
        if (k==0) return 0;
        assert(k!=0);
        return sum / k;
    }

  private:
    size_t head = 0;
    T      min_val = 0;
    T      max_val = 0;
};
