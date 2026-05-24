#pragma once 
#include "Logger.hpp"
#include <deque>
#include <mutex>
#include <unordered_set>
#include <vector>

struct ThreadPool{
    ThreadPool(std::size_t _count=1): count(_count){}
    ~ThreadPool();
    std::vector<std::jthread> threads;
    std::atomic<std::size_t> thread_id;
    std::size_t count{};
    template<typename Fn, typename ...Args>
    inline void launch(Fn&& work_fn, Args&&... params){
        for (std::size_t i=0; i<count; i++){
            threads.emplace_back(work_fn,
                                 params...);
        }
    }
};

template<typename T>
struct Queue{
    std::mutex mtx;
    std::deque<T> q;
    std::condition_variable not_empty;
    std::condition_variable not_full;
    std::size_t capacity = 1024;

    // TODO:  this is for debugging
    std::unordered_set<T> uniqueSet;


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

            uniqueSet.emplace(obj);
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
            uniqueSet.emplace(vargs...);
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
            uniqueSet.emplace(vargs...);
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
            uniqueSet.emplace(obj);
            q.push_back(std::forward<U>(obj));
        }
        not_empty.notify_one(); 
        return true;
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

        uniqueSet.erase(q.front());
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
        uniqueSet.erase(q.front());
        const auto res = std::make_optional(std::move(q.front()));
        q.pop_front();

        not_full.notify_one(); 
        return res;
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

    inline std::size_t size_unlocked(){
        return q.size();
    }

};
