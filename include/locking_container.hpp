#pragma once 
#include <mutex>
template<typename Cont>
struct locking : Cont{
    using Cont::Cont;
    using Cont::operator[];
    std::mutex mut;
    [[nodiscard("Discarding fails to preserve the lifetime of the guard, invalidating the lock")]]
    auto lock_guard(){
        return std::lock_guard(mut);
    }
};
