#pragma once 
#include <vector>
#include <unordered_map>
// fifo queue which rejects duplicate elements.
// Requires hashable element type 
// TODO: Switch out the vector for a priority queue.
// Can we maintain O(1) erasure?
// What if we just sort and reassign indices?
// OPTIONS TO REMEDY BAD ACTORS SITTING AT THE FRONT OF THE QUEUE:
// 1. Timeout system? If a chunk sits there for 5 seconds, chances are it wont be meshed anytime soon?
// 2. Sort the vector every frame before requesting
template<typename T>
struct UniqueQueue{
public:
    using value_type = T;
    std::vector<T> q;
    std::unordered_map<T, size_t> q_idx;
public:
    
    void push(T key) noexcept{
        auto it = q_idx.find(key);
        if (it != q_idx.end()){
//            LOG_ERROR("Attempt to push duplicate element onto unique queue");
        }else{
            q_idx.emplace_hint(it,key,q.size());
            q.push_back(key);
        }
    }
    void erase(T victim_key) noexcept{
        auto it = q_idx.find(victim_key);
        if (it == q_idx.end()){
            return;
        }
        const auto old_back_key = q.back();
        const auto victim_pos = it->second;
        if (q.back() != q.at(victim_pos)){
            // restore the q_idx mapping of key->pos in queue
            std::swap(q.back(),q.at(victim_pos));
            q_idx.at(old_back_key) = victim_pos;
        }
        // regardless, we need to erase the mapping from victim_key->victim_pos
        q_idx.erase(it); 
        q.pop_back();
    }

    T&& pop() {
        erase(q.back());
    }
    auto size() const noexcept{
        return q.size();
    }
    auto begin()noexcept{
        return q.begin();
    }
    auto end()noexcept{
        return q.end();
    }
    void clear() noexcept{
        q.clear();
        q_idx.clear();
    }
};
