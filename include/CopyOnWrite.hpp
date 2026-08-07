#pragma once 
#include <concepts>
#include <memory>
template<typename T>
// TODO: move methods out of line
struct COW{
    using Stored = T;
    template<typename ...Args>
        requires std::constructible_from<Stored, Args...>
    explicit constexpr COW(Args&& ...args)
        : ptr(std::make_shared<Stored>(std::forward<Args>(args)...))
    {}

    COW(const COW&) = default;
    COW& operator=(const COW&) = default;
    std::shared_ptr<Stored> ptr;
    constexpr bool is_sole_owner(){
        return ptr.use_count() == 1;
    }

    auto read() const -> const Stored&{ return *ptr; }
    auto get_snapshot()const  -> std::shared_ptr<const Stored>{ return ptr; }
    auto mutate() -> Stored&{
        if (!is_sole_owner()){
            // return a copy
            ptr = std::make_shared<Stored>(*ptr);
        }
        return *ptr;
        // otherwise, we can just return the actual value.
    }
    void publish_snapshot(Stored&& s) {
        ptr = std::make_shared<Stored>(std::move(s));
    }
};
