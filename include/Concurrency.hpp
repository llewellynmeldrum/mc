#pragma once 
#include "Logger.hpp"
#include "UIDebugLog.hpp"
#include <concepts>
#include <deque>
#include <mutex>
#include <optional>
#include <unordered_set>
#include <utility>
#include <vector>

static constexpr inline bool debug_slow_threads = false;
inline constexpr void dbg_sleep_point(){
    using namespace std::chrono_literals;
    if constexpr (debug_slow_threads){
        std::this_thread::sleep_for(250ms);
    }
};

enum struct ThreadState: i32{
#define THREAD_STATE_LIST       \
    X(STARTING)                 \
    X(WAITING_INPUT)            \
    X(WORKING)                  \
    X(WAITING_OUTPUT)           \
    X(DONE)           
#define X(val) val,
    THREAD_STATE_LIST
#undef X
};
template<> 
struct std::formatter<ThreadState>{
	constexpr auto parse(std::format_parse_context& ctx){return ctx.begin();}
	auto format(const ThreadState& val, std::format_context& ctx)const {
        std::string s{};
        switch (val){
            #define X(val) case ThreadState::val : s=#val; break;
            THREAD_STATE_LIST
            #undef X
        }
		return std::format_to(ctx.out(), "{}",s);
    }

};
struct ThreadTracker{
    ThreadTracker()= default;
    ~ThreadTracker()= default;
    ThreadTracker(const ThreadTracker& rhs) = delete;
    ThreadTracker(ThreadTracker&& rhs) = delete;
    ThreadTracker& operator=(const ThreadTracker& rhs) = delete;
    ThreadTracker& operator=(ThreadTracker&& rhs) = delete;

    std::size_t id{0};
    std::size_t count{0};
    std::string name{"N/A"};
    std::atomic<ThreadState> state{ThreadState::STARTING};
    void set_state(ThreadState s)noexcept {
        //log_to_all("Thread {}_{} state transition: [{}->{}]",name,id,state.load(),s);
        state.store(s);
    }
    std::string to_str()const {
        return std::format("{}_{}/{} is {}",name,id,count,state.load());
    }
};

template<std::size_t count=1>
struct ThreadPool{
    // Ctor does not launch threads.
    ThreadPool( std::string_view _thread_type_str = "")
        : thread_type_str(_thread_type_str){}
    // Dtor (conditionally) joins them however
    ~ThreadPool(){
        for (auto& jt: threads){
            if (jt.joinable()){
                jt.join();
            }else{
                std::println(stderr, "Unable to join thread");
                std::exit(-1);
            }
        }
    }
    std::vector<std::jthread> threads;
    std::array<ThreadTracker,count> trackers;

    std::string thread_type_str;
    template<typename Fn, typename ...Args>
    inline void launch(Fn&& work_fn, Args&&... params){
        static_assert(std::invocable<Fn,std::stop_token, ThreadTracker&, Args...>, 
        "Work function must have arg1=std::stop_token, arg2=ThreadTracker&, and argN=passed in param types.");
        for (std::size_t id=0; id<count; id++){
            auto& tracker = trackers.at(id);
            tracker.id = id+1;
            tracker.name=thread_type_str;
            tracker.count=count;
            threads.emplace_back(work_fn,
                                 std::ref(tracker),
                                 params...);
        }
    }
};

template<typename T>
struct Queue{
    std::size_t capacity = 512;
    std::mutex mtx;
    std::deque<T> q;
    std::condition_variable not_empty;
    std::condition_variable not_full;



    // BLOCKS until:
    // 1. able to grab the lock
    // 2. queue size<capacity
    template<typename U>
    requires std::same_as<std::remove_cvref_t<U>,T> && 
            std::constructible_from<T, U&&>
    inline void wait_enqueue(U&& obj){
        {
            std::unique_lock lock(mtx);
            not_full.wait(lock, [&]{
                return q.size()<capacity;
            });

            q.emplace_back(std::forward<U>(obj));
        }
        not_empty.notify_one();
    }
    // BLOCKS until:
    // 1. able to grab the lock
    // 2. queue size<capacity
    template<typename ...Args>
    inline void wait_emplace(Args&&... vargs){
        {
            std::unique_lock lock(mtx);
            not_full.wait(lock, [&](){
                return q.size()<capacity;
            });
            q.emplace_back(std::forward<Args>(vargs)...);
        }
        not_empty.notify_one();
    }

    // Succeeds only if:
    // 1. immediately able to grab the lock
    // 2. queue size<capacity
    template<typename ...Args>
    inline bool try_emplace(Args&&... vargs){
        {
            std::lock_guard lock(mtx);

            if ( !(q.size()<capacity) ){
                return false;
            }
            q.emplace_back(std::forward<Args>(vargs)...);
        }
        not_empty.notify_one(); 
        return true;
    }

    template<typename U>
    requires std::same_as<std::remove_cvref_t<U>,T> && 
            std::constructible_from<T, U&&>
    inline bool try_enqueue(U&& obj){
        {
            std::lock_guard lock(mtx);

            if ( !(q.size()<capacity) ){
                return false;
            }
            q.push_back(std::forward<U>(obj));
        }
        not_empty.notify_one(); 
        return true;
    }
    inline std::size_t slotsRemaining(){
        return capacity-q.size();
    }
    inline std::size_t try_batch_enqueue(std::span<T> batch){
        std::lock_guard lock(mtx);

        if ( slotsRemaining() < batch.size()){
            
            // cant accomodate the entire span
            return 0;
        }
        for (std::size_t i = 0; i<batch.size(); i++){
            q.push_back(batch[i]);
            not_empty.notify_one(); 
        }
        return batch.size();
    }



    // BLCOKS until:
    // 1. able to grab the lock
    // 2. queue size>0
    inline T wait_dequeue(){
        std::unique_lock lock(mtx);

        // (wait until queue is not empty)
        not_empty.wait(lock, [&](){
            return (q.empty() == false);
        });

        T res = std::move(q.front());
        q.pop_front();

        not_full.notify_one(); 
        return res;
    }

    // Succeeds only if:
    // 1. immediately able to grab the lock
    // 2. queue size>0
    inline std::optional<T> try_dequeue(){
        std::lock_guard lock(mtx);

        if (q.empty()){
            return std::nullopt;
        }
        const auto res = std::make_optional(std::move(q.front()));
        q.pop_front();

        not_full.notify_one(); 
        return res;
    }
    // Succeeds only if:
    // 1. immediately able to grab the lock
    // 2. queue size>0
    inline std::optional<std::vector<T>> try_batch_dequeue(std::size_t batch_size){
        std::lock_guard lock(mtx);

        if (q.empty()){
            return std::nullopt;
        }

        std::vector<T> batch;
        batch_size = std::min(batch_size, q.size());
        batch.reserve(batch_size);

        for (std::size_t i = 0; i<batch_size; i++){
            batch.push_back(std::move(q.front()));
            q.pop_front();
            not_full.notify_one(); 
        }

        return std::make_optional(batch);
    }



    inline bool is_empty(){
        std::lock_guard lock(mtx);
        return q.empty();
    }

    inline void wait_while_empty(){
        std::unique_lock lock(mtx);
        not_empty.wait(lock, [&](){
            return !q.empty();
        });
    }
    inline std::size_t wait_size(){
        std::lock_guard lock(mtx);
        return q.size();
    }

    inline std::size_t size_unlocked()const noexcept{
        return q.size();
    }

};
