#pragma once 
#include <deque>
#include <mutex>
#include <vector>
struct ThreadPool{
    std::vector<std::jthread> threads;
    std::atomic<std::size_t> thread_id;
    std::size_t count{};
    ThreadPool(std::size_t _count=1): count(_count){}
    template<typename Fn, typename ...Args>
    void launch(Fn&& work_fn, Args&&... params){
        for (decltype(count) i=0; i<count; i++){
            threads.emplace_back(std::forward<Fn>(work_fn),
                                 std::forward<Args>(params)...);
        }
    }
};
template<typename T>
struct Queue{
    std::mutex mtx;
    std::deque<T> q;
    std::condition_variable cv;

    void wait_enqueue_all(std::ranges::input_range auto&& r){
        std::lock_guard lock(mtx);
        for (const auto& val: r){
            q.push_back(val);
            cv.notify_one();
        }
    }
    void wait_emplace_all(std::ranges::input_range auto&& r){
        std::lock_guard lock(mtx);
        for (const auto& val: r){
            q.emplace_back(val);
            cv.notify_one(); // BUG: this may be wrong
        }
    }

    void wait_enqueue(const T& t){
        {
            std::lock_guard lock(mtx);
            q.push_back(t);
        }
        cv.notify_one();
    }

    template<typename ...Args>
    void wait_emplace(Args&&... vargs){
        {
            std::unique_lock lock(mtx);
            q.emplace_back(std::forward<Args>(vargs)...);
        }
        cv.notify_one();
    }

    template<typename ...Args>
    bool try_emplace(Args&&... vargs){
        {
            std::unique_lock lock(mtx, std::try_to_lock);

            if (!lock.owns_lock()){
                return false;
            }

            q.emplace_back(vargs...);
        }
        return true;
    }


    bool is_empty(){
        std::lock_guard lock(mtx);
        return q.empty();
    }

    void wait_while_empty(){
        std::unique_lock lock(mtx);
        cv.wait(lock, [&](){
            return !q.empty();
        });
    }

    T wait_dequeue(){
        std::unique_lock lock(mtx);

        cv.wait(lock, [&](){
            return !q.empty();
        });

        const T res = q.front();
        q.pop_front();
        return res;
    }

    // non blocking, doesnt even wait for mtx
    std::optional<T> try_dequeue(){
        std::unique_lock lock(mtx, std::try_to_lock);

        if (!lock.owns_lock() || q.empty()){
            return std::nullopt;
        }
        const auto res = std::make_optional(q.front());
        q.pop_front();
        return res;
    }

};
