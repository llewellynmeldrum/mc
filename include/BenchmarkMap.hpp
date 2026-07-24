#pragma once
#include <map>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <shared_mutex>

#include "Breakpoints.hpp"
#include "CoordTypes.hpp"
#include "Types.h"
#include "cppslop.hpp"
#include "MirroredRingBuf.hpp"
#include "FormatSpecs.hpp"

static constexpr size_t BENCHMARK_MAP_RB_SIZE = 200;


FORWARD_DECL_TEMPLATE_STRUCT(BenchmarkMap, typename K, bool ordered)

template<bool ordered, typename K, typename M>
struct resolve_map_type;

template<typename K, typename M>
struct resolve_map_type<false,K,M>{
    using type = std::unordered_map<K,M>;
};

template<typename K, typename M>
struct resolve_map_type<true,K,M>{
    using type = std::map<K,M>;
};


template<typename K, bool ordered=true>
struct BenchmarkMap {
    using key_type = K;
    template<typename mapped_type_>
    using map_type = resolve_map_type<true,key_type,mapped_type_>::type;
public:
    BenchmarkMap() = default;
    ~BenchmarkMap() = default;

    u64 framecount = 0;
    template<typename T>
        requires std::convertible_to<T,K>
    void init(const std::vector<T> e) {
        add_each(e);
        epoch = steady_clock::now(); 
    }

    void add_each(std::span<const key_type> e){
        for (const auto& v: e){
            auto [it, inserted] = map.try_emplace(v);
            assert(inserted);
        }
    }

    f64 start_of_frame = 0.0;
    f64 dt_s = 0.016;   // for the entire period between start and end frame
    using steady_clock = std::chrono::steady_clock;
    steady_clock::time_point epoch = steady_clock::now();

    f64 seconds_now(){
        auto dt = steady_clock::now() - epoch;
        auto seconds = std::chrono::duration<float>(dt);
        return seconds.count();
    }

    void start_frame() {
        f64 now = seconds_now();
        start_of_frame = now;
    }

    void bench_start(key_type const& key){
        start_time.insert_or_assign(key,seconds_now());
    }

    void bench_end(key_type const& key){
        if(map.contains(key)){
            f64 now = seconds_now();
            f64 before = start_time.at(key);
            f64 dt_s = now - before;
            map.at(key).write(static_cast<f32>(dt_s * 1000.0));
        }else{
            std::println(stderr, "key: '{}' does not exist in ringbufs.", key);
            BREAKPOINT();
        }
    }

    void end_frame(){
        f64 now = seconds_now();
        f64 before = start_of_frame;
        dt_s = now - before;
    }


    map_type<MirroredRingBuf<f32, BENCHMARK_MAP_RB_SIZE >> map{};
    map_type<f64> start_time{};

    struct ScopeTimer{
        using key_type = K;
        BenchmarkMap<key_type,ordered> & pf;
        key_type key;
        ScopeTimer(BenchmarkMap<key_type,ordered>& _prof, key_type _key) : pf(_prof), key(_key) { pf.bench_start(key);}
        ~ScopeTimer() { pf.bench_end(key);}
    };
    auto bench_scope(key_type key){return ScopeTimer{*this, key};}
private:
};

namespace std{                                                      
    template <class T1, class T2>
    struct hash<std::pair<T1,T2>>{                                                 
        constexpr std::size_t operator()(const std::pair<T1, T2>& p) const noexcept{
            auto h1 = std::hash<T1>{}(p.first);
            auto h2 = std::hash<T2>{}(p.second);
            
            // Combining the two hash values using a common mixing pattern
            return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
        }
    };
}
// todo: make a different type for ones that are meshed by only world chunk coord 
// or just make a template
template<typename K>
    requires hashable<K>
struct ChunkBenchmarkerBase{
    using RevisionID = u64;
    using key_type = K;
public:
    ChunkBenchmarkerBase() = default;
    ~ChunkBenchmarkerBase() = default;

    using steady_clock = std::chrono::steady_clock;
    steady_clock::time_point epoch = steady_clock::now();

    f64 seconds_now()const {
        auto dt = steady_clock::now() - epoch;
        auto seconds = std::chrono::duration<float>(dt);
        return seconds.count();
    }

    template<typename ...Args>
        requires std::constructible_from<key_type, Args...>
    void bench_start(Args&& ...vargs){
        key_type key = key_type(std::forward<Args>(vargs)...);
        start_times.insert_or_assign(key,seconds_now());
    }


    template<typename ...Args>
        requires std::constructible_from<key_type, Args...>
    f32 bench_end(Args&& ...vargs){
        key_type key = key_type(std::forward<Args>(vargs)...);
        auto it = start_times.find(key);
        f64 dt_s = -1000.0f;
        if(it != start_times.end()){
            f64 now = seconds_now();
            f64 before = it->second;
            dt_s = now - before;
            duration_ms.write(static_cast<f32>(dt_s * 1000.0));
        }else{
            std::println(stderr, "key: does not exist in (ChunkBenchmarker) ringbufs.");
            BREAKPOINT();
        }
        return dt_s * 1000.0;
    }



    std::unordered_map<key_type, f32> start_times{};

    constexpr static auto rb_size = 1000uz;
    MirroredRingBuf<f32, rb_size> duration_ms;
private:
};

template<typename K>
    requires hashable<K>
struct ConcurrentChunkBenchmarkerBase{
    using key_type = K;
    using RevisionID = ChunkBenchmarkerBase<key_type>::RevisionID;
    ConcurrentChunkBenchmarkerBase() = default;
    ~ConcurrentChunkBenchmarkerBase() = default;

    ChunkBenchmarkerBase<key_type> obj;
    mutable std::shared_mutex start_times_mut;
    mutable std::shared_mutex duration_mut;


    template<typename ...Args>
        requires std::constructible_from<key_type, Args...>
    void bench_start(Args&& ...vargs){
        key_type key = key_type(std::forward<Args>(vargs)...);
        std::unique_lock lock(start_times_mut);
        obj.start_times.insert_or_assign(key,obj.seconds_now());
    }


    template<typename ...Args>
        requires std::constructible_from<key_type, Args...>
    f32 bench_end(Args&& ...vargs){
        key_type key = key_type(std::forward<Args>(vargs)...);
        f64 dt_s = -1000.0f;
        f64 now = obj.seconds_now();
        dt_s = now - get_start_time(key);
        std::unique_lock lock_duration(duration_mut);
        obj.duration_ms.write(static_cast<f32>(dt_s * 1000.0));
        return dt_s * 1000.0;
    }

    template<typename ...Args>
        requires std::constructible_from<key_type, Args...>
    f32 get_start_time(Args&& ...vargs)const {
        key_type key = key_type(std::forward<Args>(vargs)...);
        std::shared_lock read_lock_start_times(start_times_mut);
        auto it = obj.start_times.find(key);
        assert(it != obj.start_times.end());
        return it->second;
    }
};

using ConcurrentChunkBenchmarker = ConcurrentChunkBenchmarkerBase<std::pair<WorldChunkCoord,u64>>;
using ChunkBenchmarker = ChunkBenchmarkerBase<std::pair<WorldChunkCoord,u64>>;
using ChunkBenchmarkerNoRevision = ChunkBenchmarkerBase<WorldChunkCoord>;
